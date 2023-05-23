
#include "opcode.h"
#include "binary.h"
#include "mov.h"
#include "reg.h"

instruction check_opcode(stream *file_stream) {
    uint8_t opcode = *(file_stream->data + file_stream->pos);

    // 6bit opcodes
    switch(opcode & 0b11111100) {
        case 0b00000000:
            // return ins6disp(opcode, fp, "add\0");
        case 0b10000000:
            // return ins_disp_data(opcode, fp);
        case 0b10001000:
            return ins6disp(opcode, &file_stream, OP_MOV);
        case 0b00101000:
            // return ins6disp(opcode, fp, "sub\0");
        case 0b00111000:
            // return ins6disp(opcode, fp, "cmp\0");
        case 0b11100000:
            // return jump_decode(opcode, fp, 1);
        break;
    }

    // 4bit opcodes
    switch(opcode & 0b11110000) {
        case 0b10110000:
            // return movIM_REG(opcode, fp);
        case 0b01110000:
            // return jump_decode(opcode, fp, 0);
        break;
    }

    // Immediate to accumulator opcodes
    switch(opcode & 0b11000110) {
        case 0b00000100:
            // return decode_immediate_accumulator(opcode, fp);
        break;
    }

    instruction op;
    op.error = 1;

    return op; // Unknown opcode
}
