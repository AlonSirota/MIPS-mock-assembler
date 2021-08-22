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
