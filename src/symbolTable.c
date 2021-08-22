//
// Created by Ram Ben on 7/2/2021.
//
#include <ctype.h>
#include "symbolTable.h"
#include "instructionList.h"


/**
 * Returns node from Symbol list (table), of parameter label
 * @param table (haystack)
 * @param label (needle)
 * @return needle
 */
Symbol *findSymbolInTable(Symbol *table, char *label) {
    while (table != NULL && strcmp(label, table->label) != 0){
        table = table->next;
    }
    return table;
}

/**
 * Adds sybol node to table (linked list).
 * If table is empty, will create the head into *tablePtr.
 * @param tablePtr to insert into
 * label, address, attributes: symbol properties.
 * @return error type
 */
enum ErrorCode addSymbol(Symbol **tablePtr, char *label, int address, int attributes) {
    Symbol* curr, *prev;
    Symbol *next;
    enum ErrorCode ec = isValidLabel(label);
    if(ec != GOOD)
        return ec;
    next = newSymbol(label, address, attributes); /* Prepare new symbol */
    if(next == NULL)
        return INSUFFICIENT_MEMORY;

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
                freeSymbolList(next); /* no need for a new node */
                return GOOD; /*  */
            }
            freeSymbolList(next);
            /*printf("label %s was already defined\n", label);*/
            return ERR_LABEL_ALREADY_DEFINED;
        }

        /* Go forward until reaching last node */
        prev = curr;
        curr = curr->next;

    } while (curr != NULL);

    /* append */
    prev->next = next;
    return GOOD;
}

/**
 * Create a new symbol with properies according to parameters
 * @return new symbol
 */
Symbol *newSymbol(char *label, int address, int attributes) {
    Symbol * s = (Symbol *) malloc(sizeof (Symbol));
    if (s == NULL)
        return NULL;
    char *nextLabel = strdup(label);
    if (nextLabel == NULL)
        return NULL;
    s->label = nextLabel;
    s->address = address;
    s->attributes = attributes;
    s->next = NULL;

    return s;
}

/*
 * Destroys the symbol linked list
 */
void freeSymbolList(Symbol *table) {
    Symbol *next;
    while(table != NULL){
        next = table->next;
        free(table->label);
        free(table);
        table = next;
    }
}

/**
 * Returns true if all is correct:
 * str is not a reserved keyword of assembly
 * str isn't empty
 * str starts with a letter
 * str only holds letters and numbers
 * str isn't longer than MAX_LABEL_LENGTH
 * @param str
 */
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
