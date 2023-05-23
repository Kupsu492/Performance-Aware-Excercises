
#include <stdio.h>
#include <stdlib.h>

// #include "main.h"
#include "opcode.h"
#include "debug.h"
#include "reg.h"
#include "binary.h"

int main(int argc, char const *argv[])
{
    FILE* fp;
    int read_failure;
    stream exec;

    instruction result;

    if (argc < 2) {
        printf("Missing bytecode file");
        return -1;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("File not found");
        return -1;
    }

    read_failure = read_executable(fp, &exec);
    fclose(fp);

    if (read_failure) {
        printf("File reading returned failure: %d", read_failure);
        return -1;
    }

    // Debug function
    if (argc > 2) {
        return printBinary(exec);
    }

    printf("\nbits 16\n\n");

    exec.pos = 0;
    do {
        result = check_opcode(&exec);
        debugPrintInstruction(result);
        if (result.error) {
            printf("Error with code: %u", result.error);
            break;
        }
    } while (exec.pos < exec.size);

    return 0;
}
