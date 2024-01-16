#ifndef _sim8086__mov
#define _sim8086__mov 1

#include <stdio.h>
#include "reg.h"
#include "main.h"

int32_t check_opcode(stream *file_stream, instruction *instruction);

int32_t ins6disp(stream *file_stream, instruction *inst);

int32_t get_effective_address(stream *file_stream, instruction *inst);

int movIM_REG(int byte, FILE* fp);

int get_value(FILE* fp, int get_word, int *failure);

int ins_disp_data(int byte, FILE* fp);

int decode_immediate_accumulator(int byte, FILE* fp);

int jump_decode(int byte, FILE* fp, int jump_table);

#endif
