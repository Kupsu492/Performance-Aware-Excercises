#ifndef _sim8086__reg
#define _sim8086__reg 1

/*	Table for register name decoding
	First 8 are 8bit registers
	Second 8 are 16bit registers */
const char field_decode[16][3] = {
	"al\0",
	"cl\0",
	"dl\0",
	"bl\0",
	"ah\0",
	"ch\0",
	"dh\0",
	"bh\0",
	"ax\0",
	"cx\0",
	"dx\0",
	"bx\0",
	"sp\0",
	"bp\0",
	"si\0",
	"di\0",
};

/*
	Table for effective address calculation */
const char ea_calc[8][10] = {
	"[bx + si]\0",
	"[bx + di]\0",
	"[bp + si]\0",
	"[bp + di]\0",
	"[si]\0    ",
	"[di]\0    ",
	"[bp]\0    ",
	"[bx]\0    ",
};
/*
	Table with direct number encoding*/
const char ea_calc_dir[8][15] = {
	"[bx + si + %u]\0",
	"[bx + di + %u]\0",
	"[bp + si + %u]\0",
	"[bp + di + %u]\0",
	"[si + %u]\0    ",
	"[di + %u]\0    ",
	"[bp + %u]\0    ",
	"[bx + %u]\0    ",
};

/*
	Table for getting instruction mnemonic
	that start with 100000 */
const char im_reg_mem_ins[8][5] = {
	"add\0",
	"\0",
	"\0",
	"\0",
	"\0",
	"sub\0",
	"\0",
	"cmp\0",
};

const char operation_size[3][6] = {
	"\0",
	"byte \0",
	"word \0",
};

const char jump_mnemonics[16][5] = {
	"jo\0",
	"jno\0",
	"jb\0",
	"jnb\0",
	"je\0",
	"jne\0",
	"jbe\0",
	"jnbe\0",
	"js\0",
	"jns\0",
	"jp\0",
	"jnp\0",
	"jl\0",
	"jnl\0",
	"jle\0",
	"jnle\0",
};

const char loop_mnemonics[4][7] = {
	"loopnz\0",
	"loopz\0",
	"loop\0",
	"jcxz\0",
};

#endif
