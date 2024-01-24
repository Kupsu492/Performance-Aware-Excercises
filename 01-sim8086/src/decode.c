
#include <stdint.h>
#include "decode.h"
#include "reg.h"

int32_t check_opcode(stream *file_stream, instruction *instruction) {
	uint8_t opcode = *(file_stream->data + file_stream->pos);
	int32_t r;

	// 7bit opcodes
	switch(opcode & 0b11111110) {
		case 0b11000110:
			instruction->op = OP_MOV;
			// instruction->oprs = REG_DATA;
			instruction->wide = (opcode & 1) ? REG_16BIT : REG_8BIT;
			r = get_EAC_with_reg(file_stream, instruction);
			if (r) return r;

			r = get_data(file_stream, &instruction->data, instruction->wide);
			if (r) return r;

			return set_oprs_immediate(instruction);
	}

	// 6bit opcodes
	switch(opcode & 0b11111100) {
		case 0b00000000:
			// return get_EAC_with_reg(opcode, fp, "add\0");
		case 0b10000000:
			// return ins_disp_data(opcode, fp);
		case 0b10001000:
			instruction->op = OP_MOV;
			instruction->dir = opcode & 2;
			instruction->wide = (opcode & 1) ? REG_16BIT : REG_8BIT;
			return get_EAC_with_reg(file_stream, instruction);
		case 0b00101000:
			// return get_EAC_with_reg(opcode, fp, "sub\0");
		case 0b00111000:
			// return get_EAC_with_reg(opcode, fp, "cmp\0");
		case 0b11100000:
			// return jump_decode(opcode, fp, 1);
		break;
	}

	// 4bit opcodes
	switch(opcode & 0b11110000) {
		case 0b10110000:
			instruction->op = OP_MOV;
			instruction->oprs = REG_IMME;
			return movREG_IM(file_stream, instruction);
		case 0b01110000:
			// return jump_decode(opcode, fp, 0);
		break;
	}

	// Immediate to accumulator opcodes
	switch(opcode & 0b11000110) {
		case 0b00000100:
			// return decode_immediate_accumulator(opcode, fp);
		break;
	}

	return 1;
}

/*
	On success return 0, if bytes are missing return 2.
*/
int32_t get_instruction_value(stream *file_stream, ins_data *value, enum wide_codes size) {
	uint16_t val = 0;
	uint8_t inc = (size == REG_16BIT) ? 2 : 1;

	if ((file_stream->pos + inc) >= file_stream->size)
		return 2; // Missing opcode's additional data bytes

	if (size == REG_16BIT) {
		val = (uint8_t) *(file_stream->data + file_stream->pos + 2);
		val <<= 8;
	}

	val |= *(file_stream->data + file_stream->pos + 1);
	file_stream->pos += inc;

	return 0;
}

int32_t decode_EAC_mod(stream *file_stream, instruction *inst) {
	uint8_t byte = *(file_stream->data + file_stream->pos);
	file_stream->pos++;

	// Get R/M
	inst->reg_mem = (byte & 0b00000111);

	switch(byte & 0b11000000) {
		case 0b11000000: // Register to register
			inst->oprs = REG_REG;
			inst->reg_mem += (uint8_t) inst->wide;
			return 0;

		case 0b10000000: // 16-bit displacement
			inst->oprs = EAC16_REG;
			get_data(file_stream, &inst->disp, true);
			break;
		case 0b01000000: // 8-bit displacement
			inst->oprs = EAC8_REG;
			get_data(file_stream, &inst->disp, false);
			// NOTE: 1 byte displacement is signed
			if (inst->disp & 0x80) {
				inst->disp ^= (int32_t) 0xFFFFFF00;
			}
			break;

		case 0b00000000: // no displacement
		default:
			if ((byte & 0b00000111) == 6) {
				// Special case: direct address mode
				inst->oprs = DIR_REG;
				inst->disp = get_data(file_stream, &inst->disp, true);
			} else {
				inst->oprs = EAC_REG;
			}
			return 0;
	}

	// This part is processing displacement cases
	if (inst->disp == 0) {
		// If data is 0, theres no need to do displacement
		// Mostly used by BP register
		inst->oprs = EAC_REG;
	}

	return 0;
}

/*
	Instruction decoding which second byte has EAC mod and r/m
	with reg.*/
int32_t get_EAC_with_reg(stream *file_stream, instruction *inst) {
	uint8_t byte;
	int32_t r;

	file_stream->pos++;
	if (file_stream->pos >= file_stream->size) {
		return 2; // Missing opcode's additional data bytes
	}

	byte = *(file_stream->data + file_stream->pos);
	inst->reg = ((byte & 0b00111000) >> 3) + (uint8_t) inst->wide;

	r = decode_EAC_mod(file_stream, inst);

	if (inst->dir && r == 0) {
		swap_direction(inst);
	}

	return r;
};

/*
	Instruction decoding which starts with 100000
	and has 3 additional opcode checks in the next byte.
	Used for SUB, ADD, CMP */
int ins_disp_data(int byte, FILE* fp) {
	const char* r_m;
	const char* ins;
	int val;
	int size = 0;
	int failure = 0;
	char eac_str[24]; // String for displacement storing

	// S field check, if uses signed numbers
	// int s = byte & 2;
	// W field check, if using 16bit registers
	int w = (byte & 1) ? 8 : 0;
	// If data is stored in 16 bits
	int d16 = (byte & 3) == 1;

	byte = fgetc(fp);
	if (feof(fp)) {
		return 2; // Missing opcode's additional data bytes
	}

	ins = im_reg_mem_ins[(byte & 0b00111000) >> 3];

	int eff = byte & 0b11000000;
	if (eff != 0b11000000) {
		// Immediate to memory
		size = (w >> 3) + 1;

		failure = decode_effective_address(&r_m, eac_str, byte, fp, w);
		if (failure)
			return failure;
	} else {
		// Immediate to register
		r_m = field_decode[(byte & 0b00000111) + w];
	}

	val = get_value(fp, d16, &failure);
	if (failure)
		return failure;

	printf("%s %s%s, %u\n", ins, operation_size[size], r_m, val);

	return 0;
}

int get_value(FILE* fp, int get_word, int *failure) {
	int val = 0;
	int byte;

	byte = fgetc(fp); // DISP-LO / Data
	if (feof(fp)) {
		*failure = 2;
		return 0; // Missing opcode's additional data bytes
	}
	val += byte;

	if (get_word) {
		byte = fgetc(fp); // DISP-HI / Data
		if (feof(fp)) {
			*failure = 2;
			return 0; // Missing opcode's additional data bytes
		}
		val += (byte << 8);
	}

	return val;
}

/**
 * Get byte or word size data
 * file_stream->pos should point to low bits
*/
int32_t get_data(stream *file_stream, int32_t *destination, bool get_word) {
	if (get_word) {
		if ((file_stream->pos + 2) > file_stream->size) {
			return 3; // Missing data bytes
		}

		int32_t val = 0;
		// First get high byte and shift it forward
		val += (uint8_t) *(file_stream->data + file_stream->pos + 1);
		val <<= 8;
		// Add lower bits
		val += (uint8_t) *(file_stream->data + file_stream->pos);

		*destination = val;
		file_stream->pos += 2;

	} else {
		if ((file_stream->pos + 1) > file_stream->size) {
			return 4; // Missing data bytes
		}

		*destination = *(file_stream->data + file_stream->pos);
		file_stream->pos++;
	}

	return 0;
}

int decode_effective_address(const char** r_m, char* eac_str, int byte, FILE* fp, int w) {
	int val;
	int r_m_field;
	int dis_16 = 0;
	int failure = 0;

	switch(byte & 0b11000000) {
		case 0b11000000: // Register to register
			*r_m = field_decode[(byte & 0b00000111) + w];
		break;

		case 0b10000000: // 16-bit displacement
			dis_16 = 1;
			__attribute__ ((fallthrough)); // For compiler: switch case fall through is desirable
		case 0b01000000: // 8-bit displacement
			r_m_field = (byte & 0b00000111);

			val = get_value(fp, dis_16, &failure);
			if (failure)
				return failure;

			if (val) {
				sprintf(eac_str, eac_disp_mnemonic[r_m_field], val);
				*r_m = eac_str;
			} else {
				*r_m = eac_mnemonic[r_m_field];
			}
			break;

		case 0b00000000: // no displacement
		default:
			if ((byte & 0b00000111) == 6) {
				// Special case: direct address mode
				val = get_value(fp, 1, &failure);
				if (failure)
					return failure;

				sprintf(eac_str, "[%u]", val);
				*r_m = eac_str;
			} else {
				*r_m = eac_mnemonic[(byte & 0b00000111)];
			}
	}

	return 0;
}

int decode_immediate_accumulator(int byte, FILE* fp) {
	const char* ins;
	const char* reg;
	int val;
	int failure;

	int w = byte & 0b00000001;
	ins = im_reg_mem_ins[(byte & 0b00111000) >> 3];

	// Register can be either ax or al
	reg = field_decode[w * 8];

	val = get_value(fp, w, &failure);

	printf("%s %s, %d\n", ins, reg, val);

	return 0;
}

int32_t movREG_IM(stream *file_stream, instruction *inst) {
	uint8_t byte = *(file_stream->data + file_stream->pos);
	file_stream->pos++;

	inst->wide = (byte & 8) ? REG_16BIT : REG_8BIT;
	inst->reg_mem = (byte & 0b00000111) + (uint8_t) inst->wide;

	return get_data(file_stream, &inst->data, inst->wide);
}

int jump_decode(int byte, FILE* fp, int table) {
	const char* ins;
	char val;


	switch(table) {
		case 0:
			ins = jump_mnemonics[byte & 0b00001111];
			break;
		case 1:
			ins = loop_mnemonics[byte & 0b00001111];
	}

	val = (char) fgetc(fp); // IP-INC8
	if (feof(fp)) {
		return 2; // Missing opcode's additional data bytes
	}

	printf("%s %i\n", ins, val);

	return 0;
}

int32_t swap_direction(instruction *inst) {
	switch(inst->oprs) {
		case REG_REG:
			inst->oprs = REG_REG_R;
			break;
		case DIR_REG:
			inst->oprs = REG_DIR;
			break;
		case EAC_REG:
			inst->oprs = REG_EAC;
			break;
		case EAC8_REG:
			inst->oprs = REG_EAC8;
			break;
		case EAC16_REG:
			inst->oprs = REG_EAC16;
			break;
		default:
			return 5;
	}
	return 0;
}

int32_t set_oprs_immediate(instruction *inst) {
	switch(inst->oprs) {
		case REG_REG:
			inst->oprs = REG_IMME;
			break;
		case DIR_REG:
			inst->oprs = DIR_IMME;
			break;
		case EAC_REG:
			inst->oprs = EAC_IMME;
			break;
		case EAC8_REG:
			inst->oprs = EAC8_IMME;
			break;
		case EAC16_REG:
			inst->oprs = EAC16_IMME;
			break;
		default:
			return 6;
	}
	return 0;
}
