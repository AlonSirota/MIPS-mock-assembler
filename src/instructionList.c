/*
// Created by Ram Ben on 7/2/2021.
*/
#include "instructionList.h"



inst *findInstruction(char *name){
    inst *ptr = instructions;
    while (ptr->IID != INSTRUCTION_LIST_END){
        if (strcmp(name, ptr->name) == 0)
            return ptr;
        ptr += 1;
    }
    return NULL;
}

int parseInstruction(node *node, int *ic, char *codeSeg, Symbol *symbolTable){
    if(node == NULL)
        return LINE_OK; /* empty line do nothing */
    inst *instruction = findInstruction(node->value);
    if(instruction == NULL)
        return LINE_UNRECOGNIZED_INSTRUCTION;
    switch (instruction->type) {
        case 'R':
            parseRInstruction(instruction, node->next, ic, codeSeg);
            break;
        case 'I':
            break;
        case 'J':
            break;
        default:
            break; /* impossible.... */
    }
}


int parseRInstruction(inst *instruction, node *node, int *ic, char *codeSeg) {
    switch (instruction->IID) {
        case INSTRUCTION_MOVE:
        case INSTRUCTION_MVLO:
        case INSTRUCTION_MVHI:
            return instructionRMove(instruction,node,ic,codeSeg);
            break;
        default:
            return instructionRArithmetic(instruction,node,ic,codeSeg);


    }
}

int instructionRArithmetic(inst *instruction, node *node, int *ic, char *codeSeg) {
    unsigned long  binaryInstruction = 0;
    int rs,rd,rt;
    rs = parseRegister(node->value);
    if (rs < 0)
        return rs;
    rd = parseRegister(node->value);
    if (rd < 0)
        return rd;
    rt = parseRegister(node->value);
    if (rt < 0)
        return rt;
    binaryInstruction += instruction->opcode << R_OP_OFFSET;
    binaryInstruction += rs << R_RS_OFFSET;
    binaryInstruction += rt << R_RT_OFFSET;
    binaryInstruction += rd << R_RD_OFFSET;
    binaryInstruction += instruction->funct << R_FUNCT_OFFSET;
    codeSeg[*ic - 100] = binaryInstruction & 15ul;
    codeSeg[*ic - 100 + 1] = binaryInstruction & 240ul;
    codeSeg[*ic - 100 + 2] = binaryInstruction & 3840ul;
    codeSeg[*ic - 100 + 3] = binaryInstruction & 61440ul;
    (*ic) += 4;
    return LINE_OK;
}

int instructionRMove(inst *instruction, node *node, int *ic, char *codeSeg) {
    unsigned long  binaryInstruction = 0;
    int rs,rd,rt;
    rs = parseRegister(node->value);
    if (rs < 0)
        return rs;
    rd = parseRegister(node->value);
    if (rd < 0)
        return rd;
    rt = 0;
    binaryInstruction += instruction->opcode << R_OP_OFFSET;
    binaryInstruction += rs << R_RS_OFFSET;
    binaryInstruction += rt << R_RT_OFFSET;
    binaryInstruction += rd << R_RD_OFFSET;
    binaryInstruction += instruction->funct << R_FUNCT_OFFSET;
    codeSeg[*ic - 100] = (binaryInstruction) & (2^8-1);
    codeSeg[*ic - 100 + 1] = (binaryInstruction >> 8) & ((2^8-1));
    codeSeg[*ic - 100 + 2] = (binaryInstruction >> 16) & ((2^8-1));
    codeSeg[*ic - 100 + 3] = (binaryInstruction >> 24) & ((2^8-1));
    (*ic) += 4;
    return LINE_OK;
}

int parseRegister(char *str) {
    if(str[0] != '$') /*todo make macro*/
        return OPERAND_NOT_REGISTER;
    if(!isdigit(str[1]))
        return OPERAND_NOT_VALID_REGISTER;
    int num = atoi(str + 1);
    if(num <0 || num > 31)
        return OPERAND_NOT_VALID_REGISTER;
    return num;
}
