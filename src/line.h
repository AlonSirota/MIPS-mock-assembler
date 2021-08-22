#ifndef ASSEMBLER_LINE_H
#define ASSEMBLER_LINE_H
#define LINE_LENGTH 80
#define WORD_DELIMITERS " \t"
#define LABEL_SUFFIX ':'

#define PARAMETER_DELIM ","

#define NONE -1

#include <assert.h>
#include <ctype.h>
#include "consts.h"
#include <stdlib.h>
#include <string.h>
#include "helper.h"

typedef struct node {
    char *value;
    struct node *next;
} node;

typedef struct line {
    char *label;
    node head;
    int error;
} line;

void parseParameters(char *paramStr, line *lOut);
void parseAscizParameters (char *paramStr, line *lOut);
void parseGenericParameters (char *paramStr, line *lOut);
line strToLine(char *str);
char * firstNoneSpace(char *str);
void trimTrailingSpace(char *str);
char * trimWhiteSpace(char *str);
void freeLine(line l);
void freeSafely(void *ptr);
int isLastNode(node n);
#endif