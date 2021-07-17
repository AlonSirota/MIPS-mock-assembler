//
// Created by alon on 7/4/21.
//

#ifndef ASSEMBLER_DIRECTIVE_H
#define ASSEMBLER_DIRECTIVE_H
#include <stdio.h>
#include <string.h>
#include "line.h"

typedef enum{
    WORD_TYPE,
    BYTE_TYPE,
    HALF_WORD_TYPE,
    ASCII_TYPE,
    UNDEFINED
} directiveType;
int isLineDirective(line l);
directiveType strToDirectiveType(char *name);

#endif //ASSEMBLER_DIRECTIVE_H
