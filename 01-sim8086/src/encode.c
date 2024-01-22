
#include <stdio.h>
#include <stdlib.h>

#include "reg.h"

void printAssemblyFile(instruction* result, size_t count) {
	const char* operation;
	const char* destination;
	const char* source;
	int32_t data;
	char eac[30];

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
			case REG_EAC:
				destination = &field_decode[result[i].destination][0];
				source = &eac_mnemonic[result[i].source][0];
				printf("%s %s, %s\n", operation, destination, source);
				break;
			case REG_EAC8:
			case REG_EAC16:
				destination = &field_decode[result[i].destination][0];
				sprintf(eac, eac_disp_mnemonic[result[i].source], result[i].disp);
				printf("%s %s, %s\n", operation, destination, eac);
				break;
			case REG_DIR:
			case REG_REG_R:
			case DIR_REG:
			case DATA_REG:
			case EAC_REG:
			case EAC8_REG:
			case EAC16_REG:
				printf("Missing operators case");
				continue;
		}
	}
	printf("\n");
}
