#ifndef _sim8086__mov
#define _sim8086__mov 1

#include <stdio.h>

int movRM_R(int data, FILE* fp);

int movIM_REG(int byte, FILE* fp);

#endif
