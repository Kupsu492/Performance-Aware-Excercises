
#include "opcode.h"

int check_opcode(int opcode, FILE* fp) {
    // 8bit opcodes

    // 7bit opcodes

    // 6bit opcodes
    switch(opcode & 0b11111100) {
        case 0b10001000:
            return movRM_R(opcode, fp);
        break;
    }

    // 4bit opcodes
    switch(opcode & 0b11110000) {
        case 0b10110000:
            return movIM_REG(opcode, fp);
        break;
    }

    return 1; // Unknown opcode
}
