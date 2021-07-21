//
// Created by alon on 7/4/21.
//

#ifndef ASSEMBLER_DIRECTIVE_H
#define ASSEMBLER_DIRECTIVE_H
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "line.h"
#define WORD_SIZE 4
#define BYTE_SIZE 1
#define HALF_SIZE 2
#define ASCII_SIZE 1

#define HALF_WORD_MIN -32768
#define HALF_WORD_MAX 32767
#define WORD_MIN -2147483648
#define WORD_MAX 2147483647
typedef unsigned char byte;
typedef enum{
    WORD,
    BYTE,
    HALF_WORD,
    ASCII,
    UNDEFINED
} directiveType;

int isLineDirective(line l);
directiveType strToDirectiveType(char *name);
byte * directiveToBytes(line l);
char* bytesToHexStr(byte bytes[]);
int lineParametersToBytes(node *head, byte *buffer, int size);
int outOfBounds(int num, int byteCount);

#endif //ASSEMBLER_DIRECTIVE_H
