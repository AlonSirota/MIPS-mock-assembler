//
// Created by alon on 8/18/21.
//

#ifndef ASSEMBLER_HELPER_H
#define ASSEMBLER_HELPER_H
#include "string.h"
#include <errno.h>
#include "consts.h"
#include "directive.h"
#include "line.h"
#include "symbolTable.h"
#include "externalTable.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>

int isEmptyString(char *str);
int parseLong(const char *str, long *val);
char *fgetsShred(FILE *f, int n, char *buffer);

#endif //ASSEMBLER_HELPER_H
