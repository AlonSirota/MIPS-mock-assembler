#include "assembler.h"

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        printf("Error: You need at least one argument");
        return 1;
    }

    /* Assemble every filename in arguments. */
    for (i = 1; i < argc; i++){
        assemblePath(argv[i]);
    }

    return 0;
}