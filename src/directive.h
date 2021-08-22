//
// Created by alon on 7/4/21.
//

#ifndef ASSEMBLER_DIRECTIVE_H
#define ASSEMBLER_DIRECTIVE_H
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "consts.h"
#include "line.h"
#define WORD_SIZE 4
#define BYTE_SIZE 1
#define HALF_SIZE 2

#define HALF_WORD_MIN -32768
#define HALF_WORD_MAX 32767
#define WORD_MIN -2147483648
#define WORD_MAX 2147483647
typedef struct byteArray {
    char *arr;
    int size;
} byteArray;

typedef enum{
    WORD_TYPE,
    BYTE_TYPE,
    HALF_WORD_TYPE,
    ASCII_TYPE,
    UNDEFINED
} directiveType;

int directiveTypeToSize(directiveType type);
int isLineDirective(struct line l);
directiveType strToDirectiveType(char *mnemonic);
byteArray directiveToBytes(struct line l, enum ErrorCode *errorOut);
int lineParametersToBytes(struct node *head, char *buffer, int size, enum ErrorCode *errorOut);
int ascizParametersToBytes(struct node *head, char *buffer, enum ErrorCode *errOut);
int outOfBounds(long num, int byteCount);

#endif //ASSEMBLER_DIRECTIVE_H
