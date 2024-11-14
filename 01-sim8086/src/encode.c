
#include <stdio.h>
#include <stdlib.h>

#include "reg.h"

void outputInstruction(instruction inst, char separator) {
	const char* operation;
	const char* reg_mem;
	const char* reg;
	const char* size;
	int32_t data, disp;
	char eac[30];

	operation = &operation_mnemonic[inst.op][0];
	switch(inst.oprs) {
		case REG_REG:
			reg_mem = &field_decode[inst.reg_mem][0];
			reg = &field_decode[inst.reg][0];
			printf("%s %s, %s%c", operation, reg_mem, reg, separator);
			break;
		case REG_IMME:
			reg = &field_decode[inst.reg][0];
			data = inst.data;
			printf("%s %s, %d%c", operation, reg, data, separator);
			break;
		case REG_EAC:
			reg = &field_decode[inst.reg][0];
			reg_mem = &eac_mnemonic[inst.reg_mem][0];
			printf("%s %s, %s%c", operation, reg, reg_mem, separator);
			break;
		case REG_EAC8:
		case REG_EAC16:
			reg = &field_decode[inst.reg][0];
			sprintf(eac, eac_disp_mnemonic[inst.reg_mem], inst.disp);
			printf("%s %s, %s%c", operation, reg, eac, separator);
			break;
		case REG_REG_R:
			reg_mem = &field_decode[inst.reg_mem][0];
			reg = &field_decode[inst.reg][0];
			printf("%s %s, %s%c", operation, reg, reg_mem, separator);
			break;
		case EAC_REG:
			reg = &field_decode[inst.reg][0];
			reg_mem = &eac_mnemonic[inst.reg_mem][0];
			printf("%s %s, %s%c", operation, reg_mem, reg, separator);
			break;
		case EAC8_REG:
		case EAC16_REG:
			reg = &field_decode[inst.reg][0];
			sprintf(eac, eac_disp_mnemonic[inst.reg_mem], inst.disp);
			printf("%s %s, %s%c", operation, eac, reg, separator);
			break;
		case EAC_IMME:
			data = inst.data;
			size = (inst.wide) ? "word" : "byte";
			reg_mem = &eac_mnemonic[inst.reg_mem][0];
			printf("%s %s, %s %d%c", operation, reg_mem, size, data, separator);
			break;
		case EAC8_IMME:
		case EAC16_IMME:
			data = inst.data;
			size = (inst.wide) ? "word" : "byte";
			sprintf(eac, eac_disp_mnemonic[inst.reg_mem], inst.disp);
			printf("%s %s, %s %d%c", operation, eac, size, data, separator);
			break;
		case REG_DIR:
			disp = inst.disp;
			reg = &field_decode[inst.reg][0];
			printf("%s %s, [%d]%c", operation, reg, disp, separator);
			break;
		case DIR_REG:
			disp = inst.disp;
			reg = &field_decode[inst.reg][0];
			printf("%s [%d], %s%c", operation, disp, reg, separator);
			break;
		case DIR_IMME:
			disp = inst.disp;
			data = inst.data;
			size = (inst.wide) ? "word" : "byte";
			printf("%s [%d], %s %d%c", operation, disp, size, data, separator);
			break;
		case IP_INC:
			data = inst.data;
			char sign = data >= 0 ? '+' : '-';
			printf("%s $%c%d+2%c", operation, sign, abs(data), separator);
	}
}
