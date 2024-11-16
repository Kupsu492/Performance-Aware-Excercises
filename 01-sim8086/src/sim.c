
#include "sim.h"
#include "reg.h"

void simulate_instruction(instruction inst, hardware *hardware) {
	const char* reg;
	int32_t last_value; // overwriten register value

	switch(inst.oprs) {
		case REG_REG:
			break;
		case REG_IMME:
			last_value = get_register_value(inst.reg, hardware);
			set_register(inst.reg, inst.data, hardware);
			reg = &field_decode[inst.reg][0];
			printf(" %s:0x%x->0x%x\n", reg, last_value, inst.data);
			break;
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
	}
}

void print_out_hardware_info(hardware *hardware) {
	printf("\n");
	printf("Final registers:\n");
	printf("\tax: 0x%04x (%d)\n", hardware->reg_ax, hardware->reg_ax);
	printf("\tbx: 0x%04x (%d)\n", hardware->reg_bx, hardware->reg_bx);
	printf("\tcx: 0x%04x (%d)\n", hardware->reg_cx, hardware->reg_cx);
	printf("\tdx: 0x%04x (%d)\n", hardware->reg_dx, hardware->reg_dx);
	printf("\tsp: 0x%04x (%d)\n", hardware->reg_sp, hardware->reg_sp);
	printf("\tbp: 0x%04x (%d)\n", hardware->reg_bp, hardware->reg_bp);
	printf("\tsi: 0x%04x (%d)\n", hardware->reg_si, hardware->reg_si);
	printf("\tdi: 0x%04x (%d)\n", hardware->reg_di, hardware->reg_di);
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
}
