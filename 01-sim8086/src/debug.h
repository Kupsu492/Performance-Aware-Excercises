#ifndef _sim8086__debug
#define _sim8086__debug 1

#include <stdio.h>
#include <stdlib.h>
#include "reg.h"
#include "main.h"

int printBinary(stream exec);

void debugPrintInstruction(instruction op);

#endif
