#ifndef _sim8086__mov
#define _sim8086__mov 1

#include <stdio.h>
#include "reg.h"
#include "main.h"

int32_t check_opcode(stream *file_stream, instruction *instruction);

int32_t get_EAC_with_reg(stream *file_stream, instruction *inst);

int32_t get_EAC_with_opcode(stream *file_stream, instruction *inst);

int32_t decode_EAC_mod(stream *file_stream, instruction *inst, uint8_t *field);

int32_t get_data(stream *file_stream, int32_t *destination, bool get_word);

int32_t swap_direction(instruction *inst);

int32_t set_oprs_immediate(instruction *inst);

enum operation decode_octal_opcode(uint8_t code);

int get_value(FILE* fp, int get_word, int *failure);

int ins_disp_data(int byte, FILE* fp);

int decode_immediate_accumulator(int byte, FILE* fp);

int jump_decode(int byte, FILE* fp, int jump_table);

enum operation get_jump(uint8_t opcode);
enum operation get_loop(uint8_t opcode);

#endif
