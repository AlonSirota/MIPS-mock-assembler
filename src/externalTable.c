//
// Created by Ram Ben on 8/16/2021.
//


#include "externalTable.h"

char *strdup(const char *src) {
    char *dst = (char *)malloc(strlen (src) + 1);  // Space for length plus nul
    if (dst == NULL) return NULL;          // No memory
    strcpy(dst, src);                      // Copy the characters
    return dst;                            // Return the new string
}


externalTable *newExternalSymbol(char *label, int address) {
    externalTable * s = (externalTable *) malloc(sizeof (externalTable));
    if (s == NULL)
        return NULL;
    char *nextLabel = strdup(label);
    if (nextLabel == NULL)
        return NULL;
    s->label = nextLabel;
    s->address = address;
    s->next = NULL;

    return s;
}


enum ErrorCode addExternal(externalTable **tablePtr, char *label, int address) {
    externalTable* curr;
    externalTable *next;
    next = newExternalSymbol(label, address); /* Prepare new symbol */
    if(next == NULL)
        return INSUFFICIENT_MEMORY;

    if (*tablePtr == NULL) { /* if table is empty */
        *tablePtr = next; /* it now contains the new element. */
        return GOOD;
    }

    /* else, find last element*/
    curr = *tablePtr;
    do {
        /* Go forward until reaching last node */
        if (curr->next != NULL)
            curr = curr->next;
    } while (curr->next != NULL);

    /* append */
    curr->next = next;
    return GOOD;
}
void discardExternalTable(externalTable *table) {
    externalTable *next;
    while(table != NULL){
        next = table->next;
        free(table->label);
        free(table);
        table = next;
    }
}
