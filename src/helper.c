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
int parseLong(const char *str, long *val) {
    char *temp;
    int rc = TRUE;
    errno = 0; /* Reset errno to avoid using a value set in the past */
    *val = strtol(str, &temp, 10);

    /* strtol error detection */
    if (temp == str || *temp != '\0' ||
        ((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE)) {
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

/**
 * Duplicate a string, with length of n
 * @param original
 * @param n
 * @return dynamically allocated string, duplicate of original
 */
char *strdupN(const char *original, int n) {
    char *out = (char *) malloc(sizeof(char) * (n + 1)); /* + 1 for '\0' */
    int i;
    for (i = 0; i < n; i++) {
        out[i] = original[i];
    }
    out[i] = '\0';
    return out;
}

/**
 * Disclaimer, copied this function online
 *
 * If *stringp is NULL, the strsep() function returns NULL and does
 * nothing else.  Otherwise, this function finds the first token in
 * the string *stringp, that is delimited by one of the bytes in the
 * string delim.  This token is terminated by overwriting the
 * dlimiter with a null byte ('\0'), and *stringp is updated to
 * point past the token.  In case no delimiter was found, the token
 * is taken to be the entire string *stringp, and *stringp is made
 * NULL.
 * @return The strsep() function returns a pointer to the token, that is, it returns the original value of *stringp.
 */
char *strsep(char **stringp, const char *delim) {
    char *start = *stringp;
    char *p;

    /* find next occurance of one of delim chars */
    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL) {
        *stringp = NULL; /* Signify reaching end of string, for next runs */
    } else {
        *p = '\0'; /* End return string at delim */
        *stringp = p + 1; /* stringP will now point to first char after delim, for next run */
    }

    return start; /* start points to the token, from stringp (before edited in this call) till delim */
}

/**
 * @param str expected to have at least one character.
 * @return last char of str
 */
char lastChar(char *str) {
    assert(str != NULL);
    assert(strlen(str));

    return str[strlen(str) - 1];
}