//
// Created by alon on 6/27/21.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#include <stdio.h>
#include <assert.h>

void assemblePath(char *fileName);
void assembleFile(FILE *f);
void firstPass(FILE *f);
#endif //ASSEMBLER_ASSEMBLER_H
