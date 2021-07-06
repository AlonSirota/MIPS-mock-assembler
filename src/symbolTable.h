//
// Created by Ram Ben on 7/3/2021.
//

#ifndef WTFPROJECT_SYMBOLTABLET_H
#define WTFPROJECT_SYMBOLTABLET_H
#include <stdlib.h>
#include "string.h"

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
int addSymbol(Symbol **tablePtr, char* label, int address, int attributes);
int discardTable(Symbol *table);



#endif //WTFPROJECT_INSTRUCTIONLIST_H
