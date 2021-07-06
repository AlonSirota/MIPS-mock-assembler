//
// Created by Ram Ben on 7/2/2021.
//
#include "symbolTable.h"


Symbol *newSymbol(char *label, int address, int attributes);

Symbol *findSymbolInTable(Symbol *table, char *label) {
    while (strcmp(label, table->label) == 0){
        table = table->next;
    }
    return table;
}


int addSymbol(Symbol **tablePtr, char *label, int address, int attributes) {
    Symbol* curr;
    Symbol *next = newSymbol(label, address, attributes); /* Prepare new symbol */
    // TODO makesure malloc succeded

    if (*tablePtr == NULL) { /* if table is empty */
        *tablePtr = next; /* it now contains the new element. */
        return EXIT_SUCCESS;
    }

    /* else, find last element*/
    curr = *tablePtr;
    while (curr->next != NULL)
        curr = curr->next; // TODO check if label already exists.

    /* append */
    curr->next = next;
    return EXIT_SUCCESS;
}

Symbol *newSymbol(char *label, int address, int attributes) {
    Symbol * new = malloc(sizeof (Symbol));
    char *nextLabel = strdup(label);
    if (nextLabel == NULL)
        return 1;
    new->label = nextLabel;
    new->address = address;
    new->attributes = attributes;
    new->next = NULL;

    return new;
}

int discardTable(Symbol *table) {
    Symbol *next;
    while(table != NULL){
        next = table->next;
        free(table->label);
        free(table);
        table = next;
    }
    return 0;
}

symbolTable emptySymbolTable() {
    symbolTable res = {.head = NULL,.last = NULL};
    return res;
}
