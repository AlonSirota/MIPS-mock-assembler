//
// Created by alon on 8/18/21.
//

#include "helper.h"

int isEmptyString(char *str) {
    return !strcmp(str, "");
}

int parseLong(const char *str, long *val)
{
    char *temp;
    int rc = TRUE;
    errno = 0;
    *val = strtol(str, &temp, 0);

    if (temp == str || *temp != '\0' ||
        ((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE))
        rc = FALSE;

    return rc;
}
