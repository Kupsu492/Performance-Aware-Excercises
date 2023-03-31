
#include <stdio.h>
#include <stdlib.h>

// #include "main.h"
#include "opcode.h"
#include "debug.h"
#include "reg.h"

int main(int argc, char const *argv[])
{
    FILE* fp;
    int opcode;
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

    // Debug function
    if (argc > 2) {
        return printBinary(fp);
    }

    printf("\nbits 16\n\n");

    while(1) {
        opcode = fgetc(fp);
        if (feof(fp)) {
            break;
        }

        result = check_opcode(opcode, fp);
        debugPrintInstruction(result);
        if (result.error) {
            printf("Error with code: %u", result.error);
            break;
        }
    }

    fclose(fp);
    return 0;
}
