
#include "sim.h"
#include "reg.h"

const int32_t FLAG_SIGN = 1;
const int32_t FLAG_ZERO = 2;

void simulate_instruction(instruction inst, hardware *hardware) {
	switch(inst.oprs) {
		case REG_REG:
			return do_reg_reg_instruction(inst, hardware);
		case REG_IMME:
			return do_reg_imme_instruction(inst, hardware);
		case REG_EAC:
		case REG_EAC8:
		case REG_EAC16:
		case REG_REG_R:
		case EAC_REG:
		case EAC8_REG:
		case EAC16_REG:
		case EAC_IMME:
		case EAC8_IMME:
		case EAC16_IMME:
		case REG_DIR:
		case DIR_REG:
		case DIR_IMME:
		case IP_INC:
			printf("Invalid/unfinished operators");
	}
}

void do_reg_reg_instruction(instruction inst, hardware *hardware) {
	const char* reg;
	int32_t value;
	int32_t last_value = get_register_value(inst.reg_mem, hardware);

	switch (inst.op) {
		case OP_ADD:
		case OP_MOV:
			value = get_register_value(inst.reg, hardware);
			set_register(inst.reg_mem, value, hardware);
			reg = &field_decode[inst.reg_mem][0];
			printf(" %s:0x%x->0x%x\n", reg, last_value, value);
			break;
		case OP_SUB:
		case OP_CMP:
		default:
			printf("Invalid operation");
	}
}

void do_reg_imme_instruction(instruction inst, hardware *hardware) {
	const char* reg = &field_decode[inst.reg][0];
	int32_t value;
	int32_t last_value = get_register_value(inst.reg, hardware);

	switch (inst.op) {
		case OP_ADD:
			value = last_value + inst.data;
			set_register(inst.reg, value, hardware);
			printf(" %s:0x%x->0x%x", reg, last_value, value);
			trigger_flags(value, hardware);
			printf("\n");
			break;
		case OP_MOV:
			set_register(inst.reg, inst.data, hardware);
			printf(" %s:0x%x->0x%x\n", reg, last_value, inst.data);
			break;
		case OP_SUB:
		case OP_CMP:
		default:
			printf("Invalid operation");
	}
}

void trigger_flags(int32_t result, hardware *hardware) {
	int32_t current_flags = hardware->flags;
	int32_t new_flags = current_flags;

	if (result == 0) {
		new_flags = new_flags | FLAG_ZERO;
	}
	if (result | 0x8000) {
		new_flags = new_flags | FLAG_SIGN;
	}

	if (current_flags != new_flags) {
		hardware->flags = new_flags;

		char sign_string1[3], sign_string2[3];
		get_flag_signs(current_flags, sign_string1);
		get_flag_signs(new_flags, sign_string2);
		printf(" flags:%s->%s", sign_string1, sign_string2);
	}
}

void get_flag_signs(int32_t signs, char *flag_string) {
	size_t i = 0;
	if (signs & FLAG_ZERO) {
		flag_string[i] = 'Z';
		i++;
	}
	if (signs & FLAG_SIGN) {
		flag_string[i] = 'S';
		i++;
	}
	flag_string[i] = '\0';
}

void print_out_hardware_info(hardware *hardware) {
	char flags[3];
	get_flag_signs(hardware->flags, flags);

	printf("\n");
	printf("Final registers:\n");
	printf("    ax: 0x%04x (%d)\n", hardware->reg_ax, hardware->reg_ax);
	printf("    bx: 0x%04x (%d)\n", hardware->reg_bx, hardware->reg_bx);
	printf("    cx: 0x%04x (%d)\n", hardware->reg_cx, hardware->reg_cx);
	printf("    dx: 0x%04x (%d)\n", hardware->reg_dx, hardware->reg_dx);
	printf("    sp: 0x%04x (%d)\n", hardware->reg_sp, hardware->reg_sp);
	printf("    bp: 0x%04x (%d)\n", hardware->reg_bp, hardware->reg_bp);
	printf("    si: 0x%04x (%d)\n", hardware->reg_si, hardware->reg_si);
	printf("    di: 0x%04x (%d)\n", hardware->reg_di, hardware->reg_di);
	printf("    di: 0x%04x (%d)\n", hardware->reg_di, hardware->reg_di);
	printf("  flags: %s\n", flags);
	printf("\n");
}

void set_register(uint8_t reg, int32_t value, hardware *hardware) {
	switch (reg) {
		case 0: // al
			hardware->reg_ax = value;
			break;
		case 1: // cl
			hardware->reg_cx = value;
			break;
		case 2: // dl
			hardware->reg_dx = value;
			break;
		case 3: // bl
			hardware->reg_bx = value;
			break;
		case 4: // ah
			hardware->reg_ax = value;
			break;
		case 5: // ch
			hardware->reg_cx = value;
			break;
		case 6: // dh
			hardware->reg_dx = value;
			break;
		case 7: // bh
			hardware->reg_bx = value;
			break;
		case 8:
			hardware->reg_ax = value;
			break;
		case 9:
			hardware->reg_cx = value;
			break;
		case 10:
			hardware->reg_dx = value;
			break;
		case 11:
			hardware->reg_bx = value;
			break;
		case 12:
			hardware->reg_sp = value;
			break;
		case 13:
			hardware->reg_bp = value;
			break;
		case 14:
			hardware->reg_si = value;
			break;
		case 15:
			hardware->reg_di = value;
			break;
		default:
			printf("\nset_register: Invalid register\n");
	}
}

int32_t get_register_value(uint8_t reg, hardware *hardware) {
	switch (reg) {
		case 0: // al
			return hardware->reg_ax;
		case 1: // cl
			return hardware->reg_cx;
		case 2: // dl
			return hardware->reg_dx;
		case 3: // bl
			return hardware->reg_bx;
		case 4: // ah
			return hardware->reg_ax;
		case 5: // ch
			return hardware->reg_cx;
		case 6: // dh
			return hardware->reg_dx;
		case 7: // bh
			return hardware->reg_bx;
		case 8:
			return hardware->reg_ax;
		case 9:
			return hardware->reg_cx;
		case 10:
			return hardware->reg_dx;
		case 11:
			return hardware->reg_bx;
		case 12:
			return hardware->reg_sp;
		case 13:
			return hardware->reg_bp;
		case 14:
			return hardware->reg_si;
		case 15:
			return hardware->reg_di;
	}

	printf("\nget_register_value: Invalid register\n");
	return 0xFFFFFFF;
}

/**
 * Set all registers to 0
 */
void init_hardware(hardware *hardware) {
	hardware->reg_ax = 0;
	hardware->reg_bx = 0;
	hardware->reg_cx = 0;
	hardware->reg_dx = 0;
	hardware->reg_sp = 0;
	hardware->reg_bp = 0;
	hardware->reg_si = 0;
	hardware->reg_di = 0;
	hardware->flags  = 0;
}
