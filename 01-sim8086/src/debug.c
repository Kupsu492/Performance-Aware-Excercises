
#include "debug.h"

// Debug function to print out file in binary numbers
int printBinary(FILE* fp) {
    int n;

    while(1) {
        n = fgetc(fp);
        if (feof(fp)) {
            return 0;
        }

        for (int i = 0; i < 8; ++i)
        {
            if (n & 128)
                printf("1");
            else
                printf("0");

            n <<= 1;
        }
        printf("\n");
    }

    return 0;
}
