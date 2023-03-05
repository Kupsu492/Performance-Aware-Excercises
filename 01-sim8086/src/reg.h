#ifndef _sim8086__reg
#define _sim8086__reg 1

/*	Table for register name decoding
	First 8 are 8bit registers
	Second 8 are 16bit registers */
char field_decode[16][2] = {
	"al",
	"cl",
	"dl",
	"bl",
	"ah",
	"ch",
	"dh",
	"bh",
	"ax",
	"cx",
	"dx",
	"bx",
	"sp",
	"bp",
	"si",
	"di",
};

#endif
