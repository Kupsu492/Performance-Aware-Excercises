#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    char file_name[30];
    int length;
    FILE *fPtr = NULL;

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



    return 0;
}
