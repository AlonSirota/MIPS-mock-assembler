//
// Created by alon on 8/14/21.
//
#include "consts.h"

ErrorString errorStrings[] = {
        {GOOD, ""},
        {GENERIC_ERROR, "Unknown error occurred"},
        {ASCIIZ_UNBALANCED_PARENTHESIS, ""},
        {ASCIIZ_MISSING_PARENTHESIS, ""},
        {ASCIIZ_EXTRA_TOKENS, ""},
        {ASCIZ_ILLEGAL_CHAR, ""},
        {MISSING_OPERAND, "Missing operand"},
        {INVALID_LABEL, "Invalid label"},
        {ERR_LABEL_ALREADY_DEFINED, "Label was already defined before"},
        {UNRECOGNIZED_INSTRUCTION, "Unrecognized instruction (instruction are lowercase ONLY)"},
        {OPERAND_NOT_REGISTER, "Operand is not a register"},
        {OPERAND_NOT_VALID_REGISTER, "Operand is not a VALID register ($0 - $31)"},
        {TOO_MANY_ARGUMENTS, "Too many arguments"},
        {ILLEAGLE_IMMED, "Argument is not a valid singed 16 bit number (immediate)"},
        {MISSING_ARGUMENTS, "Missing argument"},
        {IMMED_OUT_OF_RANGE, "immediate is out of range - 16 bit singed number"},
        {LABEL_DOES_NOT_EXIST, "Label doesn't exist"},
        {EXTERNAL_LABEL, "Label is external but instruction requires a local label"},
        {LABLE_IS_FORBIDDEN_WORD, "Label cannot be a forbidden word (instruction, etc..)"},
        {GENERIC_ERROR, NULL}
};

char *codeToMsg(enum ErrorCode ec){
    ErrorString *es = errorStrings;
    while (es->errorString != NULL){
        if (ec == es->errorCode)
            return es->errorString;
        es += 1;
    }
    return NULL;
}