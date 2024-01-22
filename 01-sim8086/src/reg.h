#ifndef _sim8086__reg
#define _sim8086__reg 1

#include <stdint.h>
#include <stdbool.h>

enum wide_codes {
	REG_8BIT = 0,
	REG_16BIT = 8,
};

extern const char field_decode[16][3];
enum register_codes {
	REG_AL,
	REG_CL,
	REG_DL,
	REG_BL,
	REG_AH,
	REG_CH,
	REG_DH,
	REG_BH,
	REG_AX,
	REG_CX,
	REG_DX,
	REG_BX,
	REG_SP,
	REG_BP,
	REG_SI,
	REG_DI,
};

extern const char operation_mnemonic[4][4];
enum operation {
	OP_MOV,
	OP_SUB,
	OP_CMP,
	OP_JMP,
};

/**
 * REG  -> register
 * DIR  -> Direct address EAC
 * DATA -> Constant value
 * EAC   -> No displacement (only registers)
 * EAC8  -> 8bit displacement
 * EAC16 -> 16bit displacement
 */
extern const char operators_mnemonic[11][11];
enum operators {
	REG_REG,
	REG_DIR,
	REG_DATA,
	REG_EAC,
	REG_EAC8,
	REG_EAC16,
	// Reversed
	REG_REG_R,
	DIR_REG,
	DATA_REG,
	EAC_REG,
	EAC8_REG,
	EAC16_REG,
};

typedef union ins_data {
	uint8_t ubyte;
	int8_t byte;
	uint16_t uword;
	int16_t word;
} ins_data;

typedef struct instruction
{
	enum operation op;
	enum operators oprs;

	// Will contain registers or effective address calculation encodings
	uint8_t reg_mem;
	uint8_t reg;

	int32_t data; // Immediate value
	int32_t disp; // EAC constant value

	enum wide_codes wide;
	bool sign;
	bool dir;
} instruction;

/*
	Table for effective address calculation */
extern const char eac_mnemonic[8][10];
/*
	Table with direct number encoding*/
extern const char eac_disp_mnemonic[8][15];

/*
	Table for getting instruction mnemonic
	that start with 100000 */
extern const char im_reg_mem_ins[8][5];

extern const char operation_size[3][6];

extern const char jump_mnemonics[16][5];

extern const char loop_mnemonics[4][7];

#endif
