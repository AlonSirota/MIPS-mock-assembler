//
// Created by Ram Ben on 7/2/2021.
//
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

Symbol *findSymbolInTable(Symbol *table, char *label){
    while (strcmp(label, table->label) == 0){
        table = table->next;
    }
    return table;
}

int addSymbol(Symbol *table, char* label, int address, int attributes){
    Symbol *next = malloc(sizeof (Symbol));
    if(next == NULL)
        return 1;
    char *nextLabel = malloc(strlen(label) * sizeof (char));
    if (nextLabel == NULL)
        return 1;
    strcpy(nextLabel, label);
    next->label = nextLabel;
    next->address = address;
    next->attributes = attributes;
    next->next = NULL;
    while (table->next != NULL)
        table = table->next;
    table->next = next;
    return 0;
}

int discardTable(Symbol *table){
    Symbol *next;
    while(table != NULL){
        next = table->next;
        free(table->label);
        free(table);
        table = next;
    }
    return 0;
}