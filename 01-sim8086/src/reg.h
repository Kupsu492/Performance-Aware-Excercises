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

extern const char operators_mnemonic[11][11];
enum operators {
	REG_REG,
	REG_DISP,
	REG_DATA,
	DISP_REG,
	DISP_DATA,
	DIR_REG,
	DIR_DATA,
	REG_DIR,
	TYPE_REG,
	TYPE_DISP,
	TYPE_DIR,
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
	uint8_t destination;
	uint8_t source;

	ins_data data; // Immediate value
	ins_data disp; // EAC constant value

	enum wide_codes wide;
	bool sign;
	bool dir;
} instruction;

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
