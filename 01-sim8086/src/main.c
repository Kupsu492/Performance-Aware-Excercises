
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "opcode.h"
#include "debug.h"
#include "reg.h"

void read_assembly(FILE *assembly, stream *exec) {
    size_t limit = 1024 * sizeof(uint8_t); // Max file size for now
    uint8_t* data = malloc(limit);

    size_t result = fread(data, sizeof(uint8_t), limit, assembly);
    exec->data = data;
    exec->size = result;
}

int main(int argc, char const *argv[])
{
    FILE* fp;
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

    read_assembly(fp, &exec);
    if (ferror(fp)) {
        printf("Something went wrong with file reading, ferror returned: %d", ferror(fp));
        fclose(fp);
        return -1;
    }
    if (feof(fp) == 0) {
        printf("File is bigger than 1kb");
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return printBinary(exec);
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
