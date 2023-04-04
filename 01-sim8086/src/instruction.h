#ifndef H_INSTRUCTION_SIM8086
#define H_INSTRUCTION_SIM8086

#include <stddef.h>
#include <stdint.h>

typedef union ins_byte {
	uint8_t ubyte;
	int8_t sbyte;
} ins_byte;

typedef union ins_word {
	uint16_t ubyte;
	int16_t sbyte;
} ins_word;

typedef struct instructions {
	size_t pos;
	size_t size;
	uint8_t* data;
} instructions;

#endif
