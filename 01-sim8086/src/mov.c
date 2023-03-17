
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

	int failure = decode_effective_address(&reg, &r_m, eac_str, byte, fp, w);
	if (failure)
		return failure;
	else if (d)
		printf("%s %s, %s\n", ins, reg, r_m);
	else
		printf("%s %s, %s\n", ins, r_m, reg);

	return 0;
};

int decode_effective_address(const char** reg, const char** r_m, char* eac_str, int byte, FILE* fp, int w) {
	int dis_16 = 0;

	switch(byte & 0b11000000) {
		case 0b11000000: // Register to register
			*reg = field_decode[((byte & 0b00111000) >> 3) + w];
			*r_m = field_decode[(byte & 0b00000111) + w];
		break;

		case 0b10000000: // 16-bit displacement
			dis_16 = 1;
			__attribute__ ((fallthrough)); // For compiler: switch case fall through is desirable
		case 0b01000000: // 8-bit displacement
			*reg = field_decode[((byte & 0b00111000) >> 3) + w];
			int r_m_field = (byte & 0b00000111);
			int val = 0;

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
				return 4;
			}

			*r_m = ea_calc[(byte & 0b00000111)];
			*reg = field_decode[((byte & 0b00111000) >> 3) + w];
	}

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
