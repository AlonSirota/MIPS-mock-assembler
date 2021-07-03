#include "line.h"

/*
 * Parse a string to a line struct
 * Returned struct contains malloced pointers and must be released.
 * ex. line = strToLine("label: a b,c ;comment")
 * line.label = "label"
 * line.head = a->b->c->NULL
 */
line strToLine(char *str) {
    line l = {.label = NULL, .head = { .value = NULL, .next = NULL} };
    char *token, *savePtr;
    node *curr;

    if (str[0] == ';') {/* Comment line, return empty line */
        return l;
    }

    token = strtok_r(str, WORD_DELIMITERS, &savePtr);
    if (token == NULL) { /* Line without words, return empty line */
        return l;
    }

    if (lastChar(token) == LABEL_SUFFIX) { /* If first word is a label */
        l.label = strdup(token);
        l.label[strlen(l.label) - 1] = '\0'; /* Trim the ':' from the label name */

        /* Next word */
        token = strtok_r(NULL, WORD_DELIMITERS, &savePtr);
        if (token == NULL) {
            return l;
        }
    }

    /* Processing next word after label, or first word if no label */
    l.head.value = strdup(token);

    curr = &l.head;
    do {
        token = strsep(&savePtr, PARAMETER_DELIM);
        if (token) {
            token = trimWhiteSpace(token);
            curr->next = (node *) malloc(sizeof (node));
            curr = curr->next;
            curr->value = strdup(token);
        }
    } while (token);
    curr->next = NULL;

    return l;
}

/*
 * Returns
 * Expects str to have at least one character.
 */
char lastChar(char *str) {
    assert(str != NULL);
    assert(strlen(str));

    return str[strlen(str) - 1];
}

/*
 * Returns pointer first none whitespace char in str.
 */
char * firstNoneSpace(char *str) {
    assert(str != NULL);
    while (*str != '\0' && isspace(*str)) {
        str++;
    }
    return str;
}

/*
 * Inserts '\0' after the last none-whitespace character.
 * Modifies parameter str.
 */
void trimTrailingSpace(char *str) {
    int i, lastGraph = NONE;
    assert(str != NULL);

    /* Find last graphical character index */
    for (i = 0; str[i] != '\0'; i++) {
        if (!isspace(str[i])) {
            lastGraph = i;
        }
    }

    if (lastGraph != NONE){ /* If there was a graphical character */
        str[lastGraph + 1] = '\0'; /* It is now the last character */
    }    
}

/*
 * Returns pointer to str without trailing and leading whitespace.
 * Points to a character in the parameter (not creating a semi-duplicate).
 */
char *trimWhiteSpace(char *str) {
    trimTrailingSpace(str);
    return firstNoneSpace(str);
}