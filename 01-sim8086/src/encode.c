
#include <stdio.h>
#include <stdlib.h>

#include "reg.h"

void printAssemblyFile(instruction* result, size_t count) {
	const char* operation;
	const char* reg_mem;
	const char* reg;
	const char* size;
	int32_t data, disp;
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
			case REG_DIR:
				disp = result[i].disp;
				reg = &field_decode[result[i].reg][0];
				printf("%s %s, [%d]\n", operation, reg, disp);
				break;
			case DIR_REG:
				disp = result[i].disp;
				reg = &field_decode[result[i].reg][0];
				printf("%s [%d], %s\n", operation, disp, reg);
				break;
			case DIR_IMME:
				disp = result[i].disp;
				data = result[i].data;
				size = (result[i].wide) ? "word" : "byte";
				printf("%s [%d], %s %d\n", operation, disp, size, data);
				break;
			case IP_INC:
				data = result[i].data;
				char sign = data >= 0 ? '+' : '-';
				printf("%s $%c%d+2\n", operation, sign, abs(data));
				// printf("Missing operators case");
		}
	}
	printf("\n");
}
