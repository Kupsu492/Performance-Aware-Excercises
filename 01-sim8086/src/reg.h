#ifndef _sim8086__reg
#define _sim8086__reg 1

#include <stdint.h>
#include <stdbool.h>

typedef struct instruction
{
	uint8_t operation;

	// Will contain registers or effective address calculation encodings
	uint8_t op1;
	uint8_t op2;

	int32_t data; // Immediate value
	int32_t disp; // EAC constant value

	bool wide;
	bool sign;
	bool dir;
	uint8_t size;

	int32_t error; // If processing failed
} instruction;

/*	Table for register name decoding
	First 8 are 8bit registers
	Second 8 are 16bit registers */
extern const char field_decode[16][3];

/*
	Table for effective address calculation */
extern const char ea_calc[8][10];
/*
	Table with direct number encoding*/
extern const char ea_calc_dir[8][15];

/*
	Table for getting instruction mnemonic
	that start with 100000 */
extern const char im_reg_mem_ins[8][5];

extern const char operation_size[3][6];

extern const char jump_mnemonics[16][5];

extern const char loop_mnemonics[4][7];

#endif
