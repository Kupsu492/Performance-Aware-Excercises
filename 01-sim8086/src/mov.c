
#include "mov.h"
#include "reg.h"

/* Register/memory to/from register instruction */
int movRM_R(int byte, FILE* fp) {
	const char* op1;
	const char* op2;
	int t;

	// int d = byte & 2;
	// If to get 16bit operator names
	int w = (byte & 1) ? 8 : 0;

	byte = fgetc(fp);
	if (feof(fp)) {
		return 2; // Missing opcode's additional data bytes
	}

	switch(byte & 0b11000000) {
		case 0b11000000: // Register to register
			op1 = field_decode[((byte & 0b00111000) >> 3) + w];
			op2 = field_decode[(byte & 0b00000111) + w];

		break;
		case 0b10000000: // Register to memory
		case 0b01000000: // Memory to register
		default:
			return 3; // Not done yet
	}

	printf("mov %s, %s\n", op2, op1);

	return 0;
};

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
