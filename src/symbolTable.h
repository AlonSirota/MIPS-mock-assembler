//
// Created by Ram Ben on 7/3/2021.
//

#ifndef ASSEMBLER_SYMBOLTABLET_H
#define ASSEMBLER_SYMBOLTABLET_H
#define MAX_LABEL_LENGTH 31


typedef struct symbolNode {
    struct symbolNode *next;
    char* label;
    int address;
    char attributes;
} Symbol;

#include "instructionList.h"
#include "consts.h"
#include "helper.h"
/* Will be used for flags as bitmask */
enum symbolType {
    UNSET = 0,
    DATA = 1,
    CODE = 1 << 1,
    ENTRY = 1 << 2,
    EXTERNAL = 1 << 3
};


Symbol *findSymbolInTable(Symbol *table, char *label);
enum ErrorCode addSymbol(Symbol **tablePtr, char* label, int address, int attributes);
void freeSymbolList(Symbol *table);
enum ErrorCode isValidLabel(char *str);
int isReservedKeyword(char *str);
Symbol *newSymbol(char *label, int address, int attributes);

#endif //ASSEMBLER_INSTRUCTIONLIST_H
