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


enum ErrorCode addSymbol(Symbol **tablePtr, char *label, int address, int attributes) {
    Symbol* curr;
    Symbol *next = newSymbol(label, address, attributes); /* Prepare new symbol */
    // TODO makesure malloc succeded

    if (*tablePtr == NULL) { /* if table is empty */
        *tablePtr = next; /* it now contains the new element. */
        return GOOD;
    }

    /* else, find last element*/
    curr = *tablePtr;
    do {
        /* Don't append to table if label already exists */
        if (!strcmp(curr->label, label)) {
            discardTable(next);
            printf("label %s was already defined\n", label);
            return ERR_LABEL_ALREADY_DEFINED;
        }

        /* Go forward until reaching last node */
        if (curr->next != NULL)
            curr = curr->next; // TODO check if label already exists.
    } while (curr->next != NULL);

    /* append */
    curr->next = next;
    return GOOD;
}

Symbol *newSymbol(char *label, int address, int attributes) {
    Symbol * s = (Symbol *) malloc(sizeof (Symbol)); // TODO handle malloc error
    char *nextLabel = strdup(label); //TODO handle strdup error
    s->label = nextLabel;
    s->address = address;
    s->attributes = attributes;
    s->next = NULL;

    return s;
}

void discardTable(Symbol *table) {
    Symbol *next;
    while(table != NULL){
        next = table->next;
        free(table->label);
        free(table);
        table = next;
    }
}