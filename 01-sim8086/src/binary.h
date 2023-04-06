
#ifndef H_BINARY_SIM8086
#define H_BINARY_SIM8086

#include <stdio.h>
#include <stdint.h>

typedef struct stream {
	size_t pos;
	size_t size;
	uint8_t* data;
} stream;

int read_executable(FILE *in, stream *exec);

#endif
