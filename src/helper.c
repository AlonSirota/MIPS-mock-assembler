//
// Created by alon on 8/18/21.
//

#include "helper.h"

/*
 * Expects str to not be null.
 * Returns TRUE if str is not "".
 */
int isEmptyString(char *str) {
    return !strcmp(str, "");
}

/*
 * Parses 'str' to long and populates 'val' with value.
 * Returns FALSE if encountered an error while parsing (number not in base 10, out of range...)
 */
int parseLong(const char *str, long *val)
{
    char *temp;
    int rc = TRUE;
    errno = 0; /* Reset errno to avoid using a value set in the past */
    *val = strtol(str, &temp, 10);

    /* strtol error detection *//* strtol error detection */
    if (temp == str || *temp != '\0' ||
        ((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE))
    {
        rc = FALSE;
    }

    return rc;
}

/*
 * Like fgets, but flushes f till end of line / EOF.
 */
char *fgetsShred(FILE *f, int n, char *buffer) {
    int i, ch, reachedEndOfLine = FALSE;

    ch = getc(f);
    if (n <= 0 || ch == EOF) { /* If nothing to be read, or EOF already reached before reading */
        return NULL;
    }
    ungetc(ch, f); /* Pushback to be read in loop later */
    n--; /* leave space fo NULL */

    for (i = 0; i < n; i++) {
        ch = getc(f);

        if (ch == EOF || ch == '\n') {
            reachedEndOfLine = TRUE;
            break;
        } else {
            buffer[i] = ch;
        }
    }
    buffer[i] = '\0';

    while (!reachedEndOfLine) { /* until reaching end of line */
        /* Shred: Read trailing characters, to avoid reading them later as a new line */
        ch = getc(f);
        if (ch == EOF || ch == '\n') {
            reachedEndOfLine = TRUE;
        }
    }

    return buffer;
}