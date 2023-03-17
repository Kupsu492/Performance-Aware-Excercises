#ifndef _sim8086__mov
#define _sim8086__mov 1

#include <stdio.h>

int ins6disp(int data, FILE* fp, char* ins);

int decode_effective_address(const char** reg, const char** r_m, char* eac_str, int byte, FILE* fp, int w);

int movIM_REG(int byte, FILE* fp);

#endif
