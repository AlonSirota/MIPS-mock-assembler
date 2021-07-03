/*
// Created by Ram Ben on 7/2/2021.
*/
#include "instructionList.h"


char parsedInstruction[4];
inst *findInstruction(char *name){
    inst *ptr = instructions;
    while (ptr->IID != INSTRUCTION_LIST_END){
        if (strcmp(name, ptr->name) == 0)
            return ptr;
        ptr += 1;
    }
    return NULL;
}

int parseInstruction(node *node, char *buf, Symbol *symbolTable, int ic) {
    if(node == NULL)
        return LINE_OK; /* empty line do nothing */
    inst *instruction = findInstruction(node->value);
    if(instruction == NULL)
        return LINE_UNRECOGNIZED_INSTRUCTION;
    switch (instruction->type) {
        case 'R':
            return parseRInstruction(instruction, node->next, buf);
            break;
        case 'I':
            return parseIInstruction(instruction, node->next, buf, symbolTable, ic);
            break;
        case 'J':
            break;
        default:
            break; /* impossible.... */
    }
}


int parseRInstruction(inst *instruction, node *node, char *buf) {
    switch (instruction->IID) {
        case INSTRUCTION_MOVE:
        case INSTRUCTION_MVLO:
        case INSTRUCTION_MVHI:
            return instructionRMove(instruction,node,buf);
            break;
        default:
            return instructionRArithmetic(instruction,node,buf);
    }
}

int instructionRArithmetic(inst *instruction, node *node, char *buf) {
    unsigned long  binaryInstruction = 0;
    unsigned int rs,rd,rt;
    rs = parseRegister(node->value);
    node = node->next;
    if (rs < 0)
        return rs;
    rt = parseRegister(node->value);
    node = node->next;
    if (rt < 0)
        return rt;
    rd = parseRegister(node->value);
    node = node->next;
    if (rd < 0)
        return rd;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction += instruction->opcode << R_OP_OFFSET;
    binaryInstruction += rs << R_RS_OFFSET;
    binaryInstruction += rt << R_RT_OFFSET;
    binaryInstruction += rd << R_RD_OFFSET;
    binaryInstruction += instruction->funct << R_FUNCT_OFFSET;
    parsedInstruction[0] = (binaryInstruction) & 0b11111111;
    parsedInstruction[1] = (binaryInstruction >> 8) & 0b11111111;
    parsedInstruction[2] = (binaryInstruction >> 16) & 0b11111111;
    parsedInstruction[3] = (binaryInstruction >> 24) & 0b11111111;
    printInstruction(buf);
    return LINE_OK;
}

int instructionRMove(inst *instruction, node *node, char *buf) {
    unsigned long  binaryInstruction = 0;
    int rs,rd,rt;
    rs = parseRegister(node->value);
    if (rs < 0)
        return rs;
    node = node->next;
    rt = parseRegister(node->value);
    if (rt < 0)
        return rt;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    rd = 0;
    binaryInstruction += instruction->opcode << R_OP_OFFSET;
    binaryInstruction += rs << R_RS_OFFSET;
    binaryInstruction += rt << R_RT_OFFSET;
    binaryInstruction += rd << R_RD_OFFSET;
    binaryInstruction += instruction->funct << R_FUNCT_OFFSET;
    parsedInstruction[0] = (binaryInstruction) & 0b11111111;
    parsedInstruction[1] = (binaryInstruction >> 8) & 0b11111111;
    parsedInstruction[2] = (binaryInstruction >> 16) & 0b11111111;
    parsedInstruction[3] = (binaryInstruction >> 24) & 0b11111111;
    printInstruction(buf);
    return LINE_OK;
}

int parseIInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic) {
    switch (instruction->IID) {
        case INSTRUCTION_BNE:
        case INSTRUCTION_BEQ:
        case INSTRUCTION_BLT:
        case INSTRUCTION_BGT:
            return instructionIBranch(instruction, node, buf, symbolTable, ic);
            break;
        case INSTRUCTION_ADDI:
        case INSTRUCTION_SUBI:
        case INSTRUCTION_ANDI:
        case INSTRUCTION_ORI:
        case INSTRUCTION_NORI:
            return instructionIArithmetic(instruction,node,buf);
            break;
        default:
            return instructionILoad(instruction,node,buf, symbolTable);


    }
}

int instructionIBranch(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic) {
    unsigned long  binaryInstruction = 0;
    int rs,rt, immed;
    rs = parseRegister(node->value);
    node = node->next;
    if (rs < 0)
        return rs;
    rt = parseRegister(node->value);
    if (rt < 0)
        return rt;
    node = node->next;
    immed = readImmed(node->value);
    binaryInstruction += instruction->opcode << I_OP_OFFSET;
    binaryInstruction += rs << I_RS_OFFSET;
    binaryInstruction += rt << I_RT_OFFSET;
    binaryInstruction += immed << R_FUNCT_OFFSET;
    parsedInstruction[0] = (binaryInstruction) & 0b11111111;
    parsedInstruction[1] = (binaryInstruction >> 8) & 0b11111111;
    parsedInstruction[2] = (binaryInstruction >> 16) & 0b11111111;
    parsedInstruction[3] = (binaryInstruction >> 24) & 0b11111111;
    printInstruction(buf);
    return LINE_OK;
}
int instructionILoad(inst *instruction, node *node, char *buf, Symbol *symbolTable){

}

int instructionIArithmetic(inst *instruction, node *node, char *buf) {
    unsigned long  binaryInstruction = 0;
    int rs,rt, immed;
    rs = parseRegister(node->value);
    node = node->next;
    if (rs < 0)
        return rs;
    rt = parseRegister(node->value);
    if (rt < 0)
        return rt;
    node = node->next;
    immed = readImmed(node->value);
    binaryInstruction += instruction->opcode << I_OP_OFFSET;
    binaryInstruction += rs << I_RS_OFFSET;
    binaryInstruction += rt << I_RT_OFFSET;
    binaryInstruction += immed << R_FUNCT_OFFSET;
    parsedInstruction[0] = (binaryInstruction) & 0b11111111;
    parsedInstruction[1] = (binaryInstruction >> 8) & 0b11111111;
    parsedInstruction[2] = (binaryInstruction >> 16) & 0b11111111;
    parsedInstruction[3] = (binaryInstruction >> 24) & 0b11111111;
    printInstruction(buf);
    return LINE_OK;
}


int parseRegister(char *str) {
    if(str == NULL){
        return MISSING_ARGUMENTS; /* todo: check empty string*/
    }
    if(str[0] != '$') /*todo make macro*/
        return OPERAND_NOT_REGISTER;
    if(!isdigit(str[1]))
        return OPERAND_NOT_VALID_REGISTER;
    int num = atoi(str + 1);
    if(num <0 || num > 31)
        return OPERAND_NOT_VALID_REGISTER;
    return num;
}
int readImmed(char *buf){
    if(buf == NULL){
        return MISSING_ARGUMENTS;
    }
    int res = 0;
    int sing = 1;
    if(!isdigit(buf[0])){
        switch (buf[0]) {
            case '-':
                sing = -1;
                break;
            case '+':
                sing = 1;
                break;
            default:
                return ILLEAGLE_IMMED; /* todo: fix range problem */
        }
    }
    while(*buf != NULL){
        if(!isdigit(buf[0]))
            return ILLEAGLE_IMMED;
        res *= 10;
        res += buf[0] - '0';
        buf++;
    }
    return res*sing;
}

void printInstruction(char *buf){
    sprintf(buf, "%.2X %.2X %.2X %.2X", parsedInstruction[0], parsedInstruction[1], parsedInstruction[2], parsedInstruction[3]);
}