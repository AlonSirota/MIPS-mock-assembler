#include "externalTable.h"


/*
 * duplicate a string to a new memory location acquired from malloc
 * */
char *strdup(const char *src) {
    char *dst = (char *)malloc(strlen (src) + 1);  /* Space for length plus nul */
    if (dst == NULL) return NULL;          /* No memory */
    strcpy(dst, src);                      /* Copy the characters */
    return dst;                            /* Return the new string */
}

/**
 * create a new externalTable node
 */
externalTable *newExternalSymbol(char *label, int address) {
    externalTable * s = (externalTable *) malloc(sizeof (externalTable));
    if (s == NULL) /* out of memort*/
        return NULL;
    char *nextLabel = strdup(label);
    if (nextLabel == NULL) /* strdup failed (out of memory)*/
        return NULL;
    s->label = nextLabel;
    s->address = address;
    s->next = NULL;

    return s;
}

/*
 * adds a new element to externals table
 * gets a pointer to externals table so it handles table initialization
 * gets parameters required for an external
 *
 * returnes GOOD on success, else the errorCode
 */
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

/*
 * free memory and destroy externals table
 */
void discardExternalTable(externalTable *table) {
    externalTable *next;
    while(table != NULL){
        next = table->next;
        free(table->label);
        free(table);
        table = next;
    }
}
