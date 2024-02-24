
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "decode.h"
#include "debug.h"
#include "encode.h"
#include "reg.h"

void read_assembly(FILE *assembly, stream *exec) {
    size_t limit = 1024 * sizeof(uint8_t); // Max file size for now
    uint8_t* data = malloc(limit);

    size_t result = fread(data, sizeof(uint8_t), limit, assembly);
    exec->data = data;
    exec->size = result;
    exec->pos = 0;
}

int main(int argc, char const *argv[])
{
    FILE* fp;
    stream exec;
    char extra_option = '0';
    int success = 0;

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

    if (argc > 2) {
        extra_option = *argv[2];
    }

    if (extra_option == 'b') {
        return printBinary(exec);
    }

    // Malloc array of 100 instructions
    instruction* result = malloc(sizeof(instruction) * MAX_DECODINGS);
    size_t count = 0;
    int32_t r;
    for (; count < MAX_DECODINGS; ++count) {
        r = check_opcode(&exec, &result[count]);
        if (r) {
            printf("Instruction decoding failed: %u\n", r);
            printf("Decoded: %lu instructions\n", count);
            success = -1;
            break;
        }

        if (exec.pos >= exec.size) {
            if (exec.pos > exec.size) {
                printf("Instruction decoding overflowed data");
                return -1;
            }
            count++;
            break;
        }
    }
    if (count == MAX_DECODINGS) {
        printf("Instruction decoding exceeded max decoded instructions");
        success = -1;
    }

    if (extra_option == 'i') {
        for (size_t i = 0; i < count; ++i)
        {
            debugPrintInstruction(result[i]);
        }
        return success;
    }

    printAssemblyFile(result, count);

    return success;
}
