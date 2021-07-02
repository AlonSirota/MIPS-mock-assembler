#include "line.h"

/*
 * Parse a string to a line struct
 * Returned struct contains malloced pointers and must be released.
 * ex. line = strToLine("label: a b,c ;comment")
 * line.label = "label"
 * line.head = a->b->c->NULL
 */
line strToLine(char *str) {
    line l = {.label = NULL, .head = {.next = NULL, .value = NULL} };
    char buffer[LINE_LENGTH + 1];
    char *token;
    node *curr, *temp;
    int i, j;

    if (str[0] == ';') {/* Comment line, return empty line */
        return l;
    }

    token = strtok(str, WORD_DELIMITERS);
    if (token == NULL) { /* Line without words, return empty line */
        return l;
    }

    if (lastChar(token) == LABEL_SUFFIX) { /* If first word is a label */
        l.label = malloc(strlen(token) + 1); //TODO malloc fail
        strcpy(l.label, token);
        l.label[strlen(l.label) - 1] = '\0'; /* Trim the ':' from the label name */

        /* Next word */
        token = strtok(NULL, WORD_DELIMITERS);
        if (token == NULL) {
            return l;
        }
    }

    /* Processing next word after label, or first word if no label */
    l.head.value = malloc(strlen(token) + 1); //TODO malloc fail
    strcpy(l.head.value, token);

    curr = &l.head;
    do {
        token = xstrtok(str, PARAMETER_DELIM);
        if (token) {
            curr->next = malloc(sizeof (node));
            curr = curr->next;
            curr->value = malloc(strlen(token) + 1);
            strcpy(curr->value, token);
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
 * Like strtok but handle empty tokens.
 * ex. xstrtok("a||b", "|") will give {"a", "", "b"}
 */
char *xstrtok(char *str, char *delims)
{
    static char *save = NULL;
    char *p;
    int n;

    if(str != NULL)
        save = str;

    /*
    *see if we have reached the end of the str
    */
    if(save == NULL || *save == '\0')
        return NULL;

    /*
     * return the number of characters that aren't delims
     */
    n = strcspn(save, delims);
    p = save; /*save start of this token*/

    save += n; /*bump past the delim*/

    if(*save != '\0') /*trash the delim if necessary*/
        *save++ = '\0';

    return(p);
}