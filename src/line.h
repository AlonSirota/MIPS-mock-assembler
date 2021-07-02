//
// Created by alon on 7/1/21.
//

#ifndef WTFPROJECT_LINE_H
#define WTFPROJECT_LINE_H
#define LINE_LENGTH 80
#define WORD_DELIMITERS " \t"
#define LABEL_SUFFIX ':'

#define PARAMETER_DELIM ","

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

line strToLine(char *str);
char lastChar(char *str);
char *xstrtok(char *str, char *delims);
#endif //WTFPROJECT_LINE_H