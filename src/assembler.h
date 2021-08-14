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
#include "directive.h"

typedef struct bytesNode {
    byte *bytes;
    struct bytesNode *next;
} bytesNode;
void assemblePath(char *fileName);
void assembleFile(FILE *f);
enum ErrorCode firstPass(FILE *f, int *icOut, int *dcOut);
enum ErrorCode secondPass(FILE *f, char* output, Symbol *st);
char *fgetsShred(FILE *f, int n, char *buffer);
void  generateOutput(FILE *f, char *codeSeg, int ic, int dc, char *dataSeg);
int addBytesToImage(bytesNode **tablePtr, byte *bytes);
enum ErrorCode processExtern(node operandHead, Symbol **symbolTablePtr, int dc);
int isLineRelevant(line l);
void printError(enum ErrorCode ec, int lineNo);
#endif //ASSEMBLER_ASSEMBLER_H
