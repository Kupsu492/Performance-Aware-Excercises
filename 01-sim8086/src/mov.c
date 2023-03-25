
#include "mov.h"
#include "reg.h"

/*
	Instruction decoding which has opcode in the first 6 bits
	and only has additional DISP bytes. */
int ins6disp(int byte, FILE* fp, char* ins) {
	const char* r_m;
	const char* reg;
	char eac_str[24]; // String for displacement storing

	// D field check, if REG field is destination
	int d = byte & 2;
	// W field check, if using 16bit data
	int w = (byte & 1) ? 8 : 0;

	byte = fgetc(fp);
	if (feof(fp)) {
		return 2; // Missing opcode's additional data bytes
	}

	reg = field_decode[((byte & 0b00111000) >> 3) + w];
	int failure = decode_effective_address(&r_m, eac_str, byte, fp, w);
	if (failure)
		return failure;
	else if (d)
		printf("%s %s, %s\n", ins, reg, r_m);
	else
		printf("%s %s, %s\n", ins, r_m, reg);

	return 0;
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

	switch(byte & 0b11000000) {
		case 0b11000000: // Register to register
			*r_m = field_decode[(byte & 0b00000111) + w];
		break;

		case 0b10000000: // 16-bit displacement
			dis_16 = 1;
			__attribute__ ((fallthrough)); // For compiler: switch case fall through is desirable
		case 0b01000000: // 8-bit displacement
			r_m_field = (byte & 0b00000111);
			val = 0;

			byte = fgetc(fp); // DISP-LO
			if (feof(fp)) {
				return 2; // Missing opcode's additional data bytes
			}
			val += byte;

			if (dis_16) {
				byte = fgetc(fp); // DISP-HI
				if (feof(fp)) {
					return 2; // Missing opcode's additional data bytes
				}
				val += (byte << 8);
			}

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
				int failure = 0;
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

	int w = byte & 0b00001000;
	int r = byte & 0b00000111;

	byte = fgetc(fp);
	if (feof(fp)) {
		return 2; // Missing opcode's additional data bytes
	}

	val += byte;

	if (w) {
		byte = fgetc(fp);
		if (feof(fp)) {
			return 2; // Missing opcode's additional data bytes
		}

		r += 8;
		val += (byte << 8);
	}

	reg = field_decode[r];

	printf("mov %s, %d\n", reg, val);

	return 0;
}
