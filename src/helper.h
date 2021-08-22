//
// Created by alon on 8/18/21.
//

#ifndef ASSEMBLER_HELPER_H
#define ASSEMBLER_HELPER_H
#include "string.h"
#include <errno.h>
#include "consts.h"
#include <stdlib.h>
#include <limits.h>

int isEmptyString(char *str);
int parseLong(const char *str, long *val);

#endif //ASSEMBLER_HELPER_H
