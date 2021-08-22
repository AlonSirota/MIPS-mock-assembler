#ifndef ASSEMBLER_HELPER_H
#define ASSEMBLER_HELPER_H
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "consts.h"
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

int isEmptyString(char *str);
int parseLong(const char *str, long *val);
char *fgetsShred(FILE *f, int n, char *buffer);
char* strsep(char** stringp, const char* delim);
char *strdupN(const char *original, int n);
char *strdup(const char *src);
char lastChar(char *str);

#endif
