//
// Created by Ram Ben on 7/3/2021.
//

#ifndef WTFPROJECT_SYMBOLTABLET_H
#define WTFPROJECT_SYMBOLTABLET_H
#include <stdlib.h>
#include "string.h"
#include "statusCodes.h"
#include "consts.h"
#include <stdio.h>

/* Will be used for flags as bitmask */
enum symbolType {
    UNSET = 0,
    DATA = 1,
    CODE = 1 << 1,
    ENTRY = 1 << 2,
    EXTERNAL = 1 << 3
};

typedef struct symbolNode {
    struct symbolNode *next;
    char* label;
    int address;
    char attributes;
} Symbol;

Symbol *findSymbolInTable(Symbol *table, char *label);
enum ErrorCode addSymbol(Symbol **tablePtr, char* label, int address, int attributes);
void discardTable(Symbol *table);
int isValidLabel(char *str);

#endif //WTFPROJECT_INSTRUCTIONLIST_H
