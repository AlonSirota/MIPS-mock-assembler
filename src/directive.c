//
// Created by Ram Ben on 7/3/2021.
//

#include "directive.h"

directiveType strToDirectiveType(char *name){
    if(name == NULL)
        return UNDEFINED;

    if (strcmp(".db", name)) {
        return BYTE;
    } else if (strcmp(".dh", name)) {
        return HALF_WORD;
    } else if (strcmp(".dw", name)) {
        return WORD;
    }else if (strcmp(".asciz", name)) {
        return ASCII;
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