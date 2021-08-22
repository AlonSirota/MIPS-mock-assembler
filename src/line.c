#include "line.h"
#include "helper.h"
#include <stddef.h>
#include <string.h>

/*
 * Parse a string to a line struct
 * Returned struct contains malloced pointers and must be released.
 * ex. line = strToLine("label: a b,c ;comment")
 * line.label = "label"
 * line.head = a->b->c->NULL
 */
line strToLine(char *str) {
    line l = {.label = NULL, .head = { .value = NULL, .next = NULL}, l.error = GOOD};
    char *token;

    if (str[0] == ';') {/* Comment line, return empty line */
        return l;
    }

    token = strtok(str, WORD_DELIMITERS);
    if (token == NULL) { /* Line without words, return empty line */
        return l;
    }

    if (lastChar(token) == LABEL_SUFFIX) { /* If first word is a label */
        l.label = strdup(token);
        l.label[strlen(l.label) - 1] = '\0'; /* Trim the ':' from the label name */

        /* Next word */
        token = strtok(NULL, WORD_DELIMITERS);
        if (token == NULL) {
            return l;
        }
    }

    /* Token is now mnemonic (either the first word, or the first word after the label  */
    l.head.value = strdup(token);

    token = strtok(NULL, ""); /* token = rest of str, that wasn't yet tokenized */
    parseParameters(token, &l);

    return l;
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

void freeLine(line l) {
    node *curr, *next;

    freeSafely(l.label);
    freeSafely(l.head.value);

    for (curr = l.head.next; curr != NULL; curr = next) {
        freeSafely(curr->value);
        next = curr->next;
        freeSafely(curr);
    }
}

void freeSafely(void *ptr) {
    if (ptr != NULL)
        free(ptr);
}

/*
 * Parse paramStr as the parameters in an .asciiz directive,
 * populate lOut's parameters accordingly.
 */
void parseAscizParameters(char *paramStr, line *lOut) {
    char *token;
    node *curr;
    token = strtok_r(NULL,"", &paramStr);
    if (*token != '\"') {
        /*printf("Expected \" as first character in first token after .asciz\n");*/
        lOut->error = ASCIIZ_MISSING_PARENTHESIS;
    } else {
        token++; /* skip first \" char */
        char *closingParenthesisPtr = strpbrk(token, "\"");
        if (closingParenthesisPtr == NULL) {
            lOut->error = ASCIIZ_UNBALANCED_PARENTHESIS;
        } else { /* Found closing \" character */
            curr = (node *) malloc(sizeof (node));
            curr->next = NULL;
            curr->value = strdupN(token, closingParenthesisPtr - token); /* -1 because we it mustn't copy the '\"' char. */
            lOut->head.next = curr;

            /* Make sure there aren't more tokens after closing parenthesis */
            trimWhiteSpace(closingParenthesisPtr);
            if (*(closingParenthesisPtr+1) != '\0') {
                lOut->error = ASCIIZ_EXTRA_TOKENS;
            }
        }
    }
}

/*
 * Parse paramStr as the parameters in the format of '[whitespace] word [whitespace],[whitespace] word...'
 * populate lOut's parameters accordingly.
 */
void parseGenericParameters(char *paramStr, line *lOut) {
    node *curr;
    char *token;
    curr = &lOut->head;
    do {
        token = strsep(&paramStr, PARAMETER_DELIM);
        if (token) {
            token = trimWhiteSpace(token);
            curr->next = (node *) malloc(sizeof (node));
            curr = curr->next;
            curr->value = strdup(token);
        }
    } while (token);
    curr->next = NULL;
}

/*
 * Parse paramStr as the parameters in the format that matches the mnemonic in lOut.
 * populate lOut's parameters accordingly.
 */
void parseParameters(char *paramStr, line *lOut) {
    /* If paramStr is empty, no parameters, this is an edge case not caught in the parse functions below */
    if (paramStr == NULL || !strcmp(paramStr, "")) {
        lOut->head.next = NULL;
    }
    else if(!strcmp(ASCII_MNEMONIC, lOut->head.value)) { /* ASCII directive requires unique line parsing */
        parseAscizParameters(paramStr, lOut);
    } else {
        parseGenericParameters(paramStr, lOut);
    }
}

/*
 * Return if node is last in linked list.
 * empty string nodes don't count as a node.
 */
int isLastNode(node n) {
    return n.next == NULL || isEmptyString(n.next->value);
}
