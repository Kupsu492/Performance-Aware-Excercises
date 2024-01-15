#ifndef _sim8086__mov
#define _sim8086__mov 1

#include <stdio.h>
#include "reg.h"
#include "main.h"

instruction check_opcode(stream *file_stream);

instruction ins6disp(uint8_t byte, stream *file_stream, enum operation op);

int decode_effective_address(const char** r_m, char* eac_str, int byte, FILE* fp, int w);

int movIM_REG(int byte, FILE* fp);

int get_value(FILE* fp, int get_word, int *failure);

int ins_disp_data(int byte, FILE* fp);

int decode_immediate_accumulator(int byte, FILE* fp);

int jump_decode(int byte, FILE* fp, int jump_table);

#endif
