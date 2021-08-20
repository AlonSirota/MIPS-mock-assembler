//
// Created by alon on 7/31/21.
//

#ifndef WTFPROJECT_CONSTS_H
#define WTFPROJECT_CONSTS_H
#include <stddef.h>
#define ASSEMBLY_EXTENSION ".as"
#define ASCII_MNEMONIC ".asciz"
#define EXTERN_MNEMONIC ".extern"
#define ENTRY_MNEMONIC ".entry"
#define FALSE 0
#define TRUE 1
#define BYTES_PER_LINE 4
#define MAX_FILE_NAME_LEN 256
enum ErrorCode {GOOD = 0, GENERIC_ERROR, ASCIIZ_UNBALANCED_PARENTHESIS, ASCIIZ_MISSING_PARENTHESIS, ASCIIZ_EXTRA_TOKENS,
        ASCIZ_ILLEGAL_CHAR, MISSING_OPERAND, INVALID_LABEL,ERR_LABEL_ALREADY_DEFINED,UNRECOGNIZED_INSTRUCTION,
    OPERAND_NOT_REGISTER, OPERAND_NOT_VALID_REGISTER,   TOO_MANY_ARGUMENTS,   ILLEAGLE_IMMED,   MISSING_ARGUMENTS,
    IMMED_OUT_OF_RANGE,    LABEL_DOES_NOT_EXIST, LABEL_NOT_CODE,   EXTERNAL_LABEL,
    FILE_WRITE_ERROR, INSUFFICIENT_MEMORY, LABEL_IS_RESERVED_KEYWORD, LABEL_TOO_LONG};

typedef struct {
    enum ErrorCode errorCode;
    char *errorString;
} ErrorString;

extern ErrorString errorStrings[];
char *codeToMsg(enum ErrorCode ec);

#endif //WTFPROJECT_CONSTS_H
