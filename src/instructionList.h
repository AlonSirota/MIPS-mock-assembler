//
// Created by Ram Ben on 7/3/2021.
//

#ifndef ASSEMBLER_INSTRUCTIONLIST_H
#define ASSEMBLER_INSTRUCTIONLIST_H

#include "directive.h"
#include "externalTable.h"
#include "symbolTable.h"

#define R_OP_OFFSET 26
#define R_RS_OFFSET 21
#define R_RT_OFFSET 16
#define R_RD_OFFSET 11
#define R_FUNCT_OFFSET 6

#define I_OP_OFFSET 26
#define I_RS_OFFSET 21
#define I_RT_OFFSET 16
#define I_IMMED_OFFSET 0

#define J_OP_OFFSET 26
#define J_REG_FLAG_OFFSET 25
#define J_ADDR_OFFSET 0

#define ONE_BYTE_CONST 255 /* one byte */

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
    const char *name;
    const char type;
    const int funct;
    const int opcode;
    const instruction_id IID; /* easier to compere insted of string */
} inst;

extern inst INSTRUCTIONS[];
inst *strToInstruction(char *name);

enum ErrorCode parseInstruction(node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1);

enum ErrorCode parseRInstruction(inst *instruction, node *node, char *buf);
enum ErrorCode parseIInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic);
enum ErrorCode parseJInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1);

enum ErrorCode instructionRArithmetic(inst *instruction, node *node, char *buf);
enum ErrorCode instructionRMove(inst *instruction, node *node, char *buf);

enum ErrorCode instructionIBranch(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic);
enum ErrorCode instructionIArithmetic(inst *instruction, node *node, char *buf);
enum ErrorCode instructionILoad(inst *instruction, node *node, char *buf);

enum ErrorCode instructionJJMP(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1);
enum ErrorCode instructionJStop(inst *instruction, node *node, char *buf);
enum ErrorCode instructionJ(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1);

enum ErrorCode parseRegister(node *node, int *reg);
void printInstruction(char *buf, unsigned int binaryInstruction);
enum ErrorCode readImmed(node *node, int *immed);
enum ErrorCode readLabel(node *node);
int to16bit(int in);
#endif //ASSEMBLER_INSTRUCTIONLIST_H
