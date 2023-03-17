#ifndef _sim8086__mov
#define _sim8086__mov 1

#include <stdio.h>

int ins6disp(int data, FILE* fp, char* ins);

int movIM_REG(int byte, FILE* fp);

#endif
