#include <stdlib.h>
#include <stdio.h>


int main(void) {
    char* name = "/home/evgenii/Desktop/msg1.txt";

    FILE* file = fopen(name, "a");


    if (file != NULL) {
        fprintf(file, "%s", "Hello file");
    }


    return 0;
}