
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
    printf("operation: %s\n", operation_mnemonic[op.op]);
    printf("destination: %s\n", field_decode[op.destination]);
    printf("source: %s\n", field_decode[op.source]);
    printf("wide: %d\n", op.wide);
    printf("sign: %d\n", op.sign);
    printf("dir: %d\n",  op.dir);
}
