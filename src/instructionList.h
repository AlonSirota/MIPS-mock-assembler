//
// Created by Ram Ben on 7/3/2021.
//

#ifndef WTFPROJECT_INSTRUCTIONLIST_H
#define WTFPROJECT_INSTRUCTIONLIST_H

#include <stdio.h>
#include <string.h>
#include "symbolTable.h"
#include "line.h"
#include "statusCodes.h"
#define R_OP_OFFSET 26
#define R_RS_OFFSET 21
#define R_RT_OFFSET 16
#define R_RD_OFFSET 11
#define R_FUNCT_OFFSET 6

#define I_OP_OFFSET 26
#define I_RS_OFFSET 21
#define I_RT_OFFSET 16
#define I_IMMED_OFFSET 0


typedef enum instruction_id {
    INSTRUCTION_ADD,
    INSTRUCTION_SUB,
    INSTRUCTION_AND,
    INSTRUCTION_OR,
    INSTRUCTION_NOR,
    INSTRUCTION_MOVE,
    INSTRUCTION_MVHI,
    INSTRUCTION_MVLO,
    INSTRUCTION_ADDI,
    INSTRUCTION_SUBI,
    INSTRUCTION_ANDI,
    INSTRUCTION_ORI,
    INSTRUCTION_NORI,
    INSTRUCTION_BNE,
    INSTRUCTION_BEQ,
    INSTRUCTION_BLT,
    INSTRUCTION_BGT,
    INSTRUCTION_LB,
    INSTRUCTION_SB,
    INSTRUCTION_LW,
    INSTRUCTION_SW,
    INSTRUCTION_LH,
    INSTRUCTION_SH,
    INSTRUCTION_JMP,
    INSTRUCTION_LA,
    INSTRUCTION_CALL,
    INSTRUCTION_STOP,
    INSTRUCTION_LIST_END
} instruction_id;
typedef struct inst{
    char *name;
    char type;
    int funct;
    int opcode;
    instruction_id IID; /* easier to compere insted of string */
} inst;

inst instructions[] = {{"add", 'R', 1, 0, INSTRUCTION_ADD},
                       {"sub", 'R', 2, 0, INSTRUCTION_SUB},
                       {"and", 'R', 3, 0, INSTRUCTION_AND},
                       {"or", 'R', 4, 0, INSTRUCTION_OR},
                       {"nor", 'R', 5, 0, INSTRUCTION_NOR},
                       {"move", 'R', 1, 1, INSTRUCTION_MOVE},
                       {"mvhi", 'R', 2, 1, INSTRUCTION_MVHI},
                       {"mvlo", 'R', 3, 1, INSTRUCTION_MVLO},
                       {"addi", 'I', 0, 10,INSTRUCTION_ADDI},
                       {"subi", 'I', 0, 11,INSTRUCTION_SUBI},
                       {"andi", 'I', 0, 12, INSTRUCTION_ANDI},
                       {"ori", 'I', 0, 13,INSTRUCTION_ORI},
                       {"nori", 'I', 0, 14, INSTRUCTION_NORI},
                       {"bne", 'I', 0, 15,INSTRUCTION_BNE},
                       {"beq", 'I', 0, 16,INSTRUCTION_BEQ},
                       {"blt", 'I', 0, 17, INSTRUCTION_BLT},
                       {"bgt", 'I', 0, 18,INSTRUCTION_BGT},
                       {"lb", 'I', 0, 19, INSTRUCTION_LB},
                       {"sb", 'I', 0, 20,INSTRUCTION_SB},
                       {"lw", 'I', 0, 21,INSTRUCTION_LW},
                       {"sw", 'I', 0, 22, INSTRUCTION_SW},
                       {"lh", 'I', 0, 23,INSTRUCTION_LH},
                       {"sh", 'I', 0, 24, INSTRUCTION_SH},
                       {"jmp", 'J', 0, 30,INSTRUCTION_JMP},
                       {"la", 'J', 0, 31,INSTRUCTION_LA},
                       {"call", 'J', 0, 32, INSTRUCTION_CALL},
                       {"stop", 'J', 0, 63,INSTRUCTION_STOP},
                       {"end of list", 0, 0, 0,INSTRUCTION_LIST_END}
};
inst *findInstruction(char *name);
int parseInstruction(node *node, char *buf, Symbol *symbolTable, int ic);
int parseRInstruction(inst *instruction, node *node, char *buf);
int instructionRArithmetic(inst *instruction, node *node, char *buf);
int instructionRMove(inst *instruction, node *node, char *buf);
int parseRegister(char *str);
void printInstruction(char *buf, int binaryInstruction);
int parseIInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic);
int instructionIBranch(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic);
int instructionIArithmetic(inst *instruction, node *node, char *buf);
int instructionILoad(inst *instruction, node *node, char *buf, Symbol *symbolTable);
int readImmed(char *buf);

#endif //WTFPROJECT_INSTRUCTIONLIST_H
