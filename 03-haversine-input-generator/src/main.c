#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    char file_name[30];

    // Try to create new data file
    for (int i = 1; i < 15; ++i)
    {
         sprintf(file_name, "data-%02d-input.json", i);
         printf("%s\n", file_name);
    }


    // if (access("./data.json", F_OK) == 0) {
    //     // file exists
    // } else {
    //     // file doesn't exist
    // }


    return 0;
}
