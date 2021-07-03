//
// Created by Ram Ben on 7/3/2021.
//

#include <stdio.h>
#include <string.h>

typedef enum{
    WORD,
    BYTE,
    HALF_WORD,
    ASCII
} guidanceID;

typedef struct guid{
    char *name;
    guidanceID GID; /* easier to compere insted of string */
} guid;

guid guidance[] = {
        {".db", BYTE},
        {".dh", HALF_WORD},
        {".dw", WORD},
        {".asciz", ASCII},
        NULL
};

guid *findGuidance(char *name){
    guid *ptr = guidance;
    while (ptr != NULL){
        if (strcmp(name, ptr->name) == 0)
            return ptr;
        ptr += 1;
    }
    return NULL;
}