//
// Created by Ram Ben on 7/2/2021.
//
#include <ctype.h>
#include "symbolTable.h"
#include "instructionList.h"


Symbol *newSymbol(char *label, int address, int attributes);

Symbol *findSymbolInTable(Symbol *table, char *label) {
    while (table != NULL && strcmp(label, table->label) != 0){
        table = table->next;
    }
    return table;
}


enum ErrorCode addSymbol(Symbol **tablePtr, char *label, int address, int attributes) {
    Symbol* curr;
    Symbol *next;
    enum ErrorCode ec = isValidLabel(label);
    // TODO check if label is legal, return an error otherwise. Can't be an assembly keyword: TEST
    if(ec != GOOD)
        return ec;
    next = newSymbol(label, address, attributes); /* Prepare new symbol */
    // TODO makesure malloc succeded

    if (*tablePtr == NULL) { /* if table is empty */
        *tablePtr = next; /* it now contains the new element. */
        return GOOD;
    }

    /* else, find last element*/
    curr = *tablePtr;
    do {
        /* Don't append to table if label already exists */
        if (!strcmp(curr->label, label) && !(attributes & EXTERNAL)) {
            if(((curr->attributes & ENTRY) || (attributes & ENTRY)) && (curr->address == -1 || address == -1)){ /* an entry may be decleared before defined, in this case temp addr "-1" is given */
                curr->attributes |= attributes; /*add new attr*/
                curr->address = (address == -1)?(curr->address):address;
                discardTable(next); /* no need for a new node */
                return GOOD; /*  */
            }
            discardTable(next);
            /*printf("label %s was already defined\n", label);*/
            return ERR_LABEL_ALREADY_DEFINED;
        }

        /* Go forward until reaching last node */
        if (curr->next != NULL)
            curr = curr->next;
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

// TODO improve documentation
enum ErrorCode isValidLabel(char *str){
    int i;
    /* Label can't be a reserved keyword */
    if (isReservedKeyword(str)) {
        return LABEL_IS_RESERVED_KEYWORD;
    }

    /* label mustn't be empty */
    if(str == NULL || isEmptyString(str))
        return INVALID_LABEL;

    /* label must start with a letter */
    if(!isalpha(str[0])) {
        return INVALID_LABEL;
    }

    /* later characters can be either letter or number */
    for (i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]))
            return INVALID_LABEL;
    }
    i--; /* don't count the last NULL character in the length */

    return (i <= MAX_LABEL_LENGTH) ? GOOD : LABEL_TOO_LONG;
}

/*
 * Returns true if 'str' is a directive or instruction mnemonic like "add" or ".asciz"
 */
int isReservedKeyword(char *str) {
    inst *instruction = strToInstruction(str);
    if(instruction != NULL)
        return TRUE;
    directiveType dt = strToDirectiveType(str);
    if (dt != UNDEFINED)
        return TRUE;

    return FALSE;
}
