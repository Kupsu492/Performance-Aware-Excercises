
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

    return 1; // Unknown opcode
}
