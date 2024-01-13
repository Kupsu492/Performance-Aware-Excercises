#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

void generate_points(FILE *fPtr, int length) {
    double max = 180.0;
    double x1, x2, y1, y2;

    fprintf(fPtr,"{\"pairs\":[");
    int i = length - 1;
    while (1) {
        x1 = ((double)rand()/(double)(RAND_MAX)) * max;
        x2 = ((double)rand()/(double)(RAND_MAX)) * max;
        y1 = ((double)rand()/(double)(RAND_MAX)) * max;
        y2 = ((double)rand()/(double)(RAND_MAX)) * max;
        
        if (i == 0) break;

        fprintf(fPtr,"{\"x0\":%lf, \"y0\":%lf, \"x1\":%lf, \"y1\":%lf},", x1, y1, x2, y2);
        i--;
    }
    fprintf(fPtr,"{\"x0\":%lf, \"y0\":%lf, \"x1\":%lf, \"y1\":%lf}]}", x1, y1, x2, y2);
}

int main(int argc, char const *argv[]) {
    char file_name[30];
    int length;
    int64_t seed;
    FILE *fPtr = NULL;

    if (argc >= 3) {
        seed = atoi(argv[2]);
    } else {
        seed = time(NULL);
    }
    srand(seed);

    if (argc < 2) {
        printf("Missing line count argument");
        return -1;
    }

    length = atoi(argv[1]);
    if (length < 1) {
        printf("Length either below 1 or not a number");
        return -1;
    }

    // Try to create a new data file
    for (int i = 1; i < 99; ++i)
    {
        sprintf(file_name, "./data-%02d-input.json", i);

        // For testing
        fPtr = fopen(file_name, "w");
        break;

        if (access(file_name, F_OK) != 0) {
            fPtr = fopen(file_name, "w");
            break;
        }
    }
    if (fPtr == NULL) {
        printf("json file couldn't be created: %s", file_name);
        return -1;
    }

    printf("Creating json file with %d lines and using seed: %ld\n", length, seed);

    generate_points(fPtr, length);

    fclose(fPtr);
    return 0;
}
