//
// Created by Ram Ben on 7/3/2021.
//

#include "directive.h"

directiveType strToDirectiveType(char *name){
    if(name == NULL)
        return UNDEFINED;

    if (strcmp(".db", name)) {
        return BYTE_TYPE;
    } else if (strcmp(".dh", name)) {
        return HALF_WORD_TYPE;
    } else if (strcmp(".dw", name)) {
        return WORD_TYPE;
    }else if (strcmp(".asciz", name)) {
        return ASCII_TYPE;
    }else {
        return UNDEFINED;
    }
}

/*
 * Returns true if line's operand is a directive.
 */
int isLineDirective(line l) {
    return strToDirectiveType(l.head.value) != UNDEFINED;
}