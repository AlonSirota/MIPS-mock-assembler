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
        assemblePath(argv[i]);
    }

    return 0;
}

void assemblePath(char *fileName) {
    // TODO enforce '.as' file extension.
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error while opening file %s\n", fileName);
    }
    assembleFile(f);
    fclose(fileName);
}

/*
 * Assembles file f.
 * f parameter is expected to be an opened file.
 * f won't be closed as part of this function.
 */
void assembleFile(FILE *f) {
    assert(f != NULL);
    firstPass(f);
    // TODO Second pass
}

void firstPass(FILE *f) {

}
