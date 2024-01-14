#ifndef _sim8086__main
#define _sim8086__main 1

#include <stdio.h>
#include <stdint.h>

typedef struct stream {
	size_t pos;
	size_t size;
	uint8_t* data;
} stream;

#endif
