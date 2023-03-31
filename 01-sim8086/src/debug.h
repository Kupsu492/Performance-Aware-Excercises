#ifndef _sim8086__debug
#define _sim8086__debug 1

#include <stdio.h>
#include <stdlib.h>
#include "reg.h"

int printBinary(FILE* fp);

void debugPrintInstruction(instruction op);

#endif
