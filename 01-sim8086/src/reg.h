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

#endif
