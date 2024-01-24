
#include <stdio.h>
#include <stdlib.h>

#include "reg.h"

void printAssemblyFile(instruction* result, size_t count) {
	const char* operation;
	const char* reg_mem;
	const char* reg;
	const char* size;
	int32_t data;
	char eac[30];

	printf("\nbits 16\n\n");
	for (size_t i = 0; i < count; ++i)
	{
		operation = &operation_mnemonic[result[i].op][0];

		switch(result[i].oprs) {
			case REG_REG:
				reg_mem = &field_decode[result[i].reg_mem][0];
				reg = &field_decode[result[i].reg][0];
				printf("%s %s, %s\n", operation, reg_mem, reg);
				break;
			case REG_IMME:
				reg = &field_decode[result[i].reg][0];
				data = result[i].data;
				printf("%s %s, %d\n", operation, reg, data);
				break;
			case REG_EAC:
				reg = &field_decode[result[i].reg][0];
				reg_mem = &eac_mnemonic[result[i].reg_mem][0];
				printf("%s %s, %s\n", operation, reg, reg_mem);
				break;
			case REG_EAC8:
			case REG_EAC16:
				reg = &field_decode[result[i].reg][0];
				sprintf(eac, eac_disp_mnemonic[result[i].reg_mem], result[i].disp);
				printf("%s %s, %s\n", operation, reg, eac);
				break;
			case REG_REG_R:
				reg_mem = &field_decode[result[i].reg_mem][0];
				reg = &field_decode[result[i].reg][0];
				printf("%s %s, %s\n", operation, reg, reg_mem);
				break;
			case EAC_REG:
				reg = &field_decode[result[i].reg][0];
				reg_mem = &eac_mnemonic[result[i].reg_mem][0];
				printf("%s %s, %s\n", operation, reg_mem, reg);
				break;
			case EAC8_REG:
			case EAC16_REG:
				reg = &field_decode[result[i].reg][0];
				sprintf(eac, eac_disp_mnemonic[result[i].reg_mem], result[i].disp);
				printf("%s %s, %s\n", operation, eac, reg);
				break;
			case EAC_IMME:
				data = result[i].data;
				size = (result[i].wide) ? "word" : "byte";
				reg_mem = &eac_mnemonic[result[i].reg_mem][0];
				printf("%s %s, %s %d\n", operation, reg_mem, size, data);
				break;
			case EAC8_IMME:
			case EAC16_IMME:
				data = result[i].data;
				size = (result[i].wide) ? "word" : "byte";
				sprintf(eac, eac_disp_mnemonic[result[i].reg_mem], result[i].disp);
				printf("%s %s, %s %d\n", operation, eac, size, data);
				break;
			case DIR_IMME:
			case REG_DIR:
			case DIR_REG:
				printf("Missing operators case");
				continue;
		}
	}
	printf("\n");
}
