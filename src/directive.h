#ifndef ASSEMBLER_DIRECTIVE_H
#define ASSEMBLER_DIRECTIVE_H
#include "line.h"
#include <limits.h>
#include <string.h>
#include "helper.h"

#define WORD_SIZE 4
#define BYTE_SIZE 1
#define HALF_SIZE 2

#define HALF_WORD_MIN -32768
#define HALF_WORD_MAX 32767
#define WORD_MIN -2147483647
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

typedef struct bytesNode {
    byteArray bytes;
    struct bytesNode *next;
} bytesNode;

int directiveTypeToSize(directiveType type);
int isLineDirective(struct line l);
directiveType strToDirectiveType(char *mnemonic);
byteArray directiveToBytes(struct line l, enum ErrorCode *errorOut);
int lineParametersToBytes(struct node *head, char *buffer, int size, enum ErrorCode *errorOut);
int ascizParametersToBytes(struct node *head, char *buffer, enum ErrorCode *errOut);
int outOfBounds(long num, int byteCount);
void freeByteList(bytesNode *head);

#endif
