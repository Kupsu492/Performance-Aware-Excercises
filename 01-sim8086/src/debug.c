
#include <stdint.h>
#include <stdbool.h>

#include "debug.h"
#include "reg.h"

// Debug function to print out file in binary numbers
int printBinary(stream exec) {
    // This is 32 bits because of bitshift throwing conversion warning
    uint32_t n;
    uint8_t *stream_n = exec.data;
    size_t e = exec.size;

    for (size_t i = 0; i < e; i++) {
        n = *stream_n;
        stream_n++;

        for (int j = 0; j < 8; ++j)
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
