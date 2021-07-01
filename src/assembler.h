//
// Created by alon on 6/27/21.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#define FALSE 0
#define TRUE 1

#include <stdio.h>
#include <assert.h>

void assemblePath(char *fileName);
void assembleFile(FILE *f);
void firstPass(FILE *f);
char *fgetsShred(FILE *f, int n, char *buffer);
#endif //ASSEMBLER_ASSEMBLER_H
