
#include "opcode.h"

int check_opcode(int opcode, FILE* fp) {
    int t = opcode;

    // First switch clause for full 8bit opcodes

    // Second switch clause for 7bit opcodes

    // Third switch clause for 6bit opcodes
    t &= 0b11111100;
    switch(t) {
        case 0b10001000:
            return movRM_R(opcode, fp);
        break;
    }

    return 1; // Unknown opcode
}
