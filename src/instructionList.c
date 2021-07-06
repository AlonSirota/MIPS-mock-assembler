/*
// Created by Ram Ben on 7/2/2021.
*/
#include "instructionList.h"

/**
 * search for an instruction
 * @param name - name of an instruction, e.g: "add", "sub"...
 * @return a pointer to the apropriate instruction, null if not found
 */
inst *findInstruction(char *name){
    inst *ptr = instructions;
    while (ptr->IID != INSTRUCTION_LIST_END){
        if (strcmp(name, ptr->name) == 0)
            return ptr;
        ptr += 1;
    }
    return NULL;
}

/**
 * parses an instuction line
 * @param node - the first element in a line
 * @param buf - string buffer to write instruction in hex format
 * @param symbolTable  - lable symbolNode table
 * @param ic - the instruction counter
 * @return the parsing result: LINE_OK if line is gramatically correct, else returns the error in the line
 */
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

/**
 * parse spesifically R typed instruction
 * parsing is grouped by similar types of instructions
 * @param instruction
 * @param node
 * @param buf
 * @return
 */
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
    unsigned long  binaryInstruction = 0; /* the resulting binary code of the instruction */
    unsigned int rs,rd,rt; /* registers */
    rs = parseRegister(node->value);
    node = node->next;
    if (rs < 0) /* register number can only be non negetive, negetive result means an error was returned */
        return rs;
    rt = parseRegister(node->value);
    node = node->next;
    if (rt < 0)
        return rt;
    rd = parseRegister(node->value);
    node = node->next;
    if (rd < 0)
        return rd;
    if(node != NULL) /* didnt reach end of line */
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << R_OP_OFFSET; /* shifting parameters to the defined position */
    binaryInstruction |= rs << R_RS_OFFSET;
    binaryInstruction |= rt << R_RT_OFFSET;
    binaryInstruction |= rd << R_RD_OFFSET;
    binaryInstruction |= instruction->funct << R_FUNCT_OFFSET;
    printInstruction(buf, binaryInstruction);
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
    printInstruction(buf, binaryInstruction);
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
    int rs,rt, immed; /* immed is 16 bit singed int*/
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
    printInstruction(buf, binaryInstruction);
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
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}


/**
 * recives one parameter from line and tries to parse register id
 * @param str  - the parameter that should contain a register number, can be null in which case a MISSING_ARGUMENT error will be returned
 * @return the register id or the appropriate error code
 */
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

/**
 * recives one parameter from line and tries to parse a valid immediate
 * @param buf  - the parameter that should contain a valid immediate, can be null in which case a MISSING_ARGUMENT error will be returned
 * @return the value of the immediate or the appropriate error code
 */
int readImmed(char *buf){
    if(buf == NULL || strlen(buf)==0){
        return MISSING_ARGUMENTS;
    }
    int prev = 0, res = 0;
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
                return ILLEAGLE_IMMED;
        }
    }
    while(*buf != NULL){
        if(!isdigit(buf[0]))
            return ILLEAGLE_IMMED;
        prev = res;
        res *= 10;
        res += buf[0] - '0';
        if(res <= prev) /* int overflow (int may be bigger then 16 bits)*/
            return IMMED_OUT_OF_RANGE;
        buf++;
    }
    res *= sing;
    if(res < (-(1 << 16)) || res > ((1 << 16) - 1)) /* res is 16 bit singed so its range in 2's complimint is: -2^15 <= res <= 2^15-1 */
        return IMMED_OUT_OF_RANGE;
    return res*sing;
}

/**
 * parses the internal instuction buffer into buf with the required format
 * @param buf - outup string buffer
 */
void printInstruction(char *buf, int binaryInstruction) {
    char parsedInstruction[4];
    parsedInstruction[0] = (binaryInstruction) & 0b11111111; /* extructing bytes from instruction to "memory" in little endian */
    parsedInstruction[1] = (binaryInstruction >> 8) & 0b11111111;
    parsedInstruction[2] = (binaryInstruction >> 16) & 0b11111111;
    parsedInstruction[3] = (binaryInstruction >> 24) & 0b11111111;
    sprintf(buf, "%.2X %.2X %.2X %.2X", parsedInstruction[0], parsedInstruction[1], parsedInstruction[2], parsedInstruction[3]);
}