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

/*
 * Convert l line (that is assumed to be a directive command) to byte array.
 */
byte * directiveToBytes(line l) {
    char *mnemonic = l.head.value;
    byte bytes[LINE_LENGTH * WORD_SIZE]; /* This size will suffice the 'heaviest' case of a line like "<mnemonic> .dw 0,0,0,0..." */
    int count;
    byte *result;

    if (mnemonic == NULL) {
        return NULL;
    }
    else if (!strcmp(mnemonic, ".db")) {
        count = lineParametersToBytes(l.head.next, bytes, BYTE_SIZE);
    }else if (!strcmp(mnemonic, ".dh")) {
        count = lineParametersToBytes(l.head.next, bytes, HALF_SIZE);
    }else if (!strcmp(mnemonic, ".dw")) {
        count = lineParametersToBytes(l.head.next, bytes, WORD_SIZE);
    }
    else if (!strcmp(mnemonic, ".asciz")) {

    }else  {
        printf("Unrecognized directive mnemonic: \"%s\"\n", mnemonic);
    }

    /* Copy the section of the buffer that contains the generated data. */
    result = (byte *) malloc(sizeof(byte) * count); /* TODO error check */
    memcpy(result, bytes, count);
    return result;
}

/*
 * Convert linked list of nodes that represent parameters of a command, to bytes.
 * The bytes are inserted to the buffer (which is assumed to have enough room)
 * Bytes are inserted in little endian
 * Returns the number of bytes written.
 */
int lineParametersToBytes(node *head, byte *buffer, int size) {
    long int res;
    int i, j, count = 0, shift;
    for (i = 0;head != NULL; head = head->next, i++) {
        assert(head->value != NULL);
        res = atol(head->value);
        if (errno != 0) {
            printf("parameter \"%s\" isn't a valid number", head->value);
            return 0;
        }
        else if (outOfBounds(res, size)) {
            printf("parameter \"%s\" is out of range\n", head->value);
            return 0;
        }

        // little endian?!?!?!
        for (j = 0, shift = 0; j < size; j++, shift += CHAR_BIT) {
            res = res >> shift;
            buffer[i * size + j] = res & 0b11111111;
            count++;
        }
    }

    return count;
}

/*
 * Returns true if number isn't in the range of numbers that can be
 * represented by byteCount bytes.
 */
int outOfBounds(int num, int byteCount) {
    int min, max;
    switch (byteCount) {
        case 1:
            min = CHAR_MIN;
            max = CHAR_MAX;
            break;
        case 2:
            min = HALF_WORD_MIN;
            max = HALF_WORD_MAX;
            break;
        case 4:
            min = WORD_MIN;
            max = WORD_MAX;
            break;
        default:
            printf("Error, unrecognized byte size in 'outOfBounds' function\n");
    }
    return num > max || num < min;
}
