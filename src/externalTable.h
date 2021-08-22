//
// Created by Ram Ben on 8/16/2021.
//

#ifndef ASSEMBLER_EXTERNALTABLE_H
#define ASSEMBLER_EXTERNALTABLE_H
#include "consts.h"
#include <string.h>
#include <stdlib.h>
typedef struct externalTableNode {
    struct externalTableNode *next;
    char* label;
    int address;
} externalTable;

enum ErrorCode addExternal(externalTable **tablePtr, char* label, int address);
void discardExternalTable(externalTable *table);



#endif /*ASSEMBLER_EXTERNALTABLE_H */
