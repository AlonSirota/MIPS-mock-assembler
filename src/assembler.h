//
// Created by alon on 6/27/21.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#define USING_WINDOWS
#define CODE_SIZE 2^25 /* max memory size */
#define DATA_SIZE 2^25

#include <stdio.h>
#include <assert.h>
#include "directive.h"
#include "line.h"
#include "symbolTable.h"
#include "directive.h"
#include "externalTable.h"

typedef struct bytesNode {
    byte *bytes;
    struct bytesNode *next;
} bytesNode;

char parseOp(node node, char string[18], int i, Symbol *pSymbol);
void logError(enum ErrorCode error, int *hasErrors, int lineNumber);
char *codeToMsg(enum ErrorCode code);
void assemblePath(char *fileName);
void assembleFile(FILE *asFile, char *fileName);
enum ErrorCode firstPass(FILE *asFile, int *icOut, int *dcOut, bytesNode **dataImagePtr, Symbol **symbolTableOut);
enum ErrorCode secondPass(FILE *f, FILE *objFile, Symbol *st, externalTable  *externalTable1);
char *fgetsShred(FILE *f, int n, char *buffer);
void  generateOutput(FILE *f, char *codeSeg, int ic, int dc, char *dataSeg);
int addBytesToImage(bytesNode **tablePtr, byte *bytes);
enum ErrorCode processExtern(node operandHead, Symbol **symbolTablePtr);
int isLineRelevant(line l);
void printError(enum ErrorCode ec, int lineNo);
enum ErrorCode printLineToFile(FILE *pIobuf, int no, char *buf);
enum ErrorCode writeObjFileHeader(FILE *pIobuf, int ic, int dc);
enum ErrorCode generateEntriesFile(char *fileName, Symbol *symbolTable);
enum ErrorCode generateExternalsFile (char *fileName, externalTable *et);
#endif //ASSEMBLER_ASSEMBLER_H
