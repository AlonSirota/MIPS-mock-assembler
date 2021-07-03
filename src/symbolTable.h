//
// Created by Ram Ben on 7/3/2021.
//

#ifndef WTFPROJECT_SYMBOLTABLET_H
#define WTFPROJECT_SYMBOLTABLET_H
#include <stdlib.h>
#include "string.h"
#define SYMBOL_DATA 1
#define SYMBOL_CODE 2
#define SYMBOL_ENTRY 4
#define SYMBOL_EXTERNAL 8

typedef struct symbol {
    struct symbol *next;
    char* label;
    int address;
    int attributes;
} Symbol;

Symbol *findSymbolInTable(Symbol *table, char *label);
int addSymbol(Symbol *table, char* label, int address, int attributes);
int discardTable(Symbol *table);



#endif //WTFPROJECT_INSTRUCTIONLIST_H
