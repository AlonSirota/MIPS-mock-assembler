//
// Created by alon on 6/27/21.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#define CODE_SIZE (2^25) /* max memory size */

#define ERROR_BUFFER_LENGTH 256

#define FIRST_MEMORY_ADDRESS 100

#include <stdio.h>
#include <assert.h>
#include "directive.h"
#include "line.h"
#include "symbolTable.h"
#include "directive.h"
#include "externalTable.h"
#include "helper.h"
#include "instructionList.h"

typedef struct bytesNode {
    byteArray bytes;
    struct bytesNode *next;
} bytesNode;

void logError(enum ErrorCode error, int *hasErrors, int lineNumber);
void assemblePath(char *fileName);
void assembleFile(FILE *asFile, char *fileName);
enum ErrorCode firstPass(FILE *asFile, int *icOut, int *dcOut, bytesNode **dataImagePtr, Symbol **symbolTableOut);
enum ErrorCode secondPass(FILE *asFile, FILE *objFile, Symbol *st, externalTable  **externalTable1);
enum ErrorCode addBytesToImage(bytesNode **tablePtr, byteArray bytes);
enum ErrorCode processExtern(node *operandHead, Symbol **symbolTablePtr);
int isLineRelevant(line l);
void printError(enum ErrorCode ec, int lineNo);
enum ErrorCode printLineToFile(FILE *pIobuf, int no, char *buf);
enum ErrorCode writeObjFileHeader(FILE *pIobuf, int ic, int dc);
enum ErrorCode generateEntriesFile(char *fileName, Symbol *symbolTable);
enum ErrorCode generateExternalsFile (char *fileName, externalTable *et);
enum ErrorCode appendDataImageToFile(FILE *objFile, bytesNode *dataImage, int ic);
void removeOutputFiles(char *baseName);
enum ErrorCode processDirective(line lineParsed,bytesNode **dataImagePtr, Symbol **symbolTableOut, int *dcOut, int *hasErrors, int lineNumber);
#endif //ASSEMBLER_ASSEMBLER_H
