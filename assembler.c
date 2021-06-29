//
// Created by alon on 6/27/21.
//
#include "assembler.h"

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        printf("Error: You need at least one argument");
        return 1;
    }

    /* Assemble every filename in arguments. */
    for (i = 1; i < argc; i++){
        assemble(argv[i]);
    }

    return 0;
}

void assemble(char *fileName) {
    /* TODO */
}
