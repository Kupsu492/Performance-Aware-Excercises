
#include <stdint.h>
#include "decode.h"
#include "reg.h"

int32_t check_opcode(stream *file_stream, instruction *instruction) {
    uint8_t opcode = *(file_stream->data + file_stream->pos);

    // 6bit opcodes
    switch(opcode & 0b11111100) {
        case 0b00000000:
            // return ins6disp(opcode, fp, "add\0");
        case 0b10000000:
            // return ins_disp_data(opcode, fp);
        case 0b10001000:
            instruction->op = OP_MOV;
            return ins6disp(file_stream, instruction);
        case 0b00101000:
            // return ins6disp(opcode, fp, "sub\0");
        case 0b00111000:
            // return ins6disp(opcode, fp, "cmp\0");
        case 0b11100000:
            // return jump_decode(opcode, fp, 1);
        break;
    }

    // 4bit opcodes
    switch(opcode & 0b11110000) {
        case 0b10110000:
            // return movIM_REG(opcode, fp);
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

int32_t get_effective_address(stream *file_stream, instruction *inst) {
	uint8_t byte = *(file_stream->data + file_stream->pos);

	switch(byte & 0b11000000) {
		case 0b11000000: // Register to register
			inst->oprs = REG_REG;
			inst->source = (byte & (uint8_t) 0b00000111) + (uint8_t) inst->wide;
			file_stream->pos++;
		break;

		// case 0b10000000: // 16-bit displacement
		// 	size = REG_16BIT;
		// 	__attribute__ ((fallthrough)); // For compiler: switch case fall through is desirable
		// case 0b01000000: // 8-bit displacement
		// 	type = TYPE_DISP;
		// 	inst->op2 = (byte & 0b00000111);
		// 	inst->error = get_instruction_value(file_stream, &inst->disp, size);
		// 	break;

		// case 0b00000000: // no displacement
		default:
			return 32;
		// 	if ((byte & 0b00000111) == 6) {
		// 		// Special case: direct address mode
		// 		type = TYPE_DIR;
		// 		inst->error = get_instruction_value(file_stream, &inst->disp, REG_16BIT);
		// 	} else {
		// 		type = TYPE_DISP;
		// 		inst->op2 = (byte & 0b00000111);
		// 	}
	}

	return 0;
}

/*
	Instruction decoding which has opcode in the first 6 bits
	and only has additional DISP bytes. */
int32_t ins6disp(stream *file_stream, instruction *inst) {
	uint8_t byte = *(file_stream->data + file_stream->pos);
	int32_t r;

	inst->dir = byte & 2;
	inst->wide = (byte & 1) ? REG_16BIT : REG_8BIT;

	file_stream->pos++;
	if (file_stream->pos >= file_stream->size) {
		return 2; // Missing opcode's additional data bytes
	}

	byte = *(file_stream->data + file_stream->pos);
	inst->destination = ((byte & 0b00111000) >> 3) + (uint8_t) inst->wide;

	r = get_effective_address(file_stream, inst);

	if (inst->dir == 0) {
		byte = inst->destination;
		inst->destination = inst->source;
		inst->source = byte;
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
				sprintf(eac_str, ea_calc_dir[r_m_field], val);
				*r_m = eac_str;
			} else {
				*r_m = ea_calc[r_m_field];
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
				*r_m = ea_calc[(byte & 0b00000111)];
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

int movIM_REG(int byte, FILE* fp) {
	const char* reg;
	int val = 0;
	int failure = 0;

	int w = byte & 0b00001000;

	reg = field_decode[(byte & 0b00000111) + w];

	val = get_value(fp, w, &failure);
	if (failure)
		return failure;

	printf("mov %s, %d\n", reg, val);

	return 0;
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
