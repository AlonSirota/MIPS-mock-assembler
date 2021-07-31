//
// Created by alon on 7/1/21.
//

#ifndef WTFPROJECT_LINE_H
#define WTFPROJECT_LINE_H
#define LINE_LENGTH 80
#define WORD_DELIMITERS " \t"
#define LABEL_SUFFIX ':'

#define PARAMETER_DELIM ","

#define NONE -1

#include "consts.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    char *value;
    struct node *next;
} node;

typedef struct line {
    char *label;
    node head;
} line;

char* strsep(char** stringp, const char* delim);
line strToLine(char *str);
char lastChar(char *str);
char * firstNoneSpace(char *str);
void trimTrailingSpace(char *str);
char * trimWhiteSpace(char *str);
void freeLine(line l);
void freeSafely(void *ptr);
#endif //WTFPROJECT_LINE_H