
#include <stdio.h>
#include <stdlib.h>

#include "reg.h"

void printAssemblyFile(instruction* result, size_t count) {
	const char* operation;
	const char* destination;
	const char* source;
	int32_t data;

	printf("\nbits 16\n\n");
	for (size_t i = 0; i < count; ++i)
	{
		operation = &operation_mnemonic[result[i].op][0];

		switch(result[i].oprs) {
			case REG_REG:
				destination = &field_decode[result[i].destination][0];
				source = &field_decode[result[i].source][0];
				printf("%s %s, %s\n", operation, destination, source);
				break;
			case REG_DATA:
				destination = &field_decode[result[i].destination][0];
				data = result[i].data;
				printf("%s %s, %d\n", operation, destination, data);
				break;
			case REG_DISP:
			case DISP_REG:
			case DISP_DATA:
			case DIR_REG:
			case DIR_DATA:
			case REG_DIR:
			case TYPE_REG:
			case TYPE_DISP:
			case TYPE_DIR:
				printf("Broken decoded instruction: operator");
				continue;
		}
	}
	printf("\n");
}
