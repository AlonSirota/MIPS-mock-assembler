//
// Created by alon on 6/27/21.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#define USING_WINDOWS
#define FALSE 0
#define TRUE 1

#include <stdio.h>
#include <assert.h>
#include "directive.h"
#include "line.h"
#include "symbolTable.h"

void assemblePath(char *fileName);
void assembleFile(FILE *f);
void firstPass(FILE *f);
void secondPass(FILE *f);
char *fgetsShred(FILE *f, int n, char *buffer);
void  generateOutput(FILE *f, char *codeSeg, int ic, int dc, char *dataSeg);
#endif //ASSEMBLER_ASSEMBLER_H
