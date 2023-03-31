
#include <stdint.h>
#include <stdbool.h>

#include "debug.h"
#include "reg.h"

// Debug function to print out file in binary numbers
int printBinary(FILE* fp) {
    int n;

    while(1) {
        n = fgetc(fp);
        if (feof(fp)) {
            return 0;
        }

        for (int i = 0; i < 8; ++i)
        {
            if (n & 128)
                printf("1");
            else
                printf("0");

            n <<= 1;
        }
        printf("\n");
    }

    return 0;
}

void debugPrintInstruction(instruction op) {
    printf("operation: %d\n", op.operation);

    printf("op1: %d\n", op.op1);
    printf("op2: %d\n", op.op2);

    printf("wide: %d\n", op.wide);
    printf("sign: %d\n", op.sign);
    printf("dir: %d\n",  op.dir);
    printf("size: %d\n", op.size);

    printf("error: %d\n", op.error);
}
