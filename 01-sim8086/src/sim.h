#ifndef _sim8086__sim
#define _sim8086__sim value

#include <stdio.h>
#include <stdlib.h>

#include "reg.h"

typedef struct hardware {
	int32_t reg_ax;
	int32_t reg_bx;
	int32_t reg_cx;
	int32_t reg_dx;
	int32_t reg_sp;
	int32_t reg_bp;
	int32_t reg_si;
	int32_t reg_di;
} hardware;

void simulate_instruction(instruction inst, hardware *hardware);
void set_register(uint8_t reg, int32_t value, hardware *hardware);
void print_out_hardware_info(hardware *hardware);
int32_t get_register_value(uint8_t reg, hardware *hardware);
void do_reg_reg_instruction(instruction inst, hardware *hardware);
void do_reg_imme_instruction(instruction inst, hardware *hardware);
void init_hardware(hardware *hardware);

#endif
