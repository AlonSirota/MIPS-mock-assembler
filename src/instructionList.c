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
        case 'I':
            return parseIInstruction(instruction, node->next, buf, symbolTable, ic);
        case 'J':
            return parseJInstruction(instruction, node->next, buf, symbolTable);
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
    int rs,rd,rt; /* registers */
    rs = parseRegister(node);
    if (rs < 0) /* register number can only be non negetive, negetive result means an error was returned */
        return rs;
    node = node->next;
    rt = parseRegister(node);
    if (rt < 0)
        return rt;
    node = node->next;
    rd = parseRegister(node);
    if (rd < 0)
        return rd;
    node = node->next;
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

int instructionRMove(inst *instruction, node *node, char *buf) { /* move rd, rs */
    unsigned long  binaryInstruction = 0;
    int rs,rd,rt;
    rs = parseRegister(node);
    if (rs < 0)
        return rs;
    node = node->next;
    rd = parseRegister(node);
    if (rd < 0)
        return rd;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    rt = 0;
    binaryInstruction |= instruction->opcode << R_OP_OFFSET;
    binaryInstruction |= rs << R_RS_OFFSET;
    binaryInstruction |= rt << R_RT_OFFSET;
    binaryInstruction |= rd << R_RD_OFFSET;
    binaryInstruction |= instruction->funct << R_FUNCT_OFFSET;
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
    int rs,rt, add, resCode;
    char *label;
    Symbol *s;
    rs = parseRegister(node);
    if (rs < 0)
        return rs;
    node = node->next;
    rt = parseRegister(node);
    if (rt < 0)
        return rt;
    node = node->next;
    resCode = readLabel(node);
    if(resCode)
        return resCode;
    label = node->value;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    s = findSymbolInTable(symbolTable, label);
    if(s == NULL)
        return LABEL_DOES_NOT_EXIST;
    if(!(s->attributes & CODE))
        return LABEL_NOT_CODE;
    add = s->address - ic;
    binaryInstruction |= instruction->opcode << I_OP_OFFSET;
    binaryInstruction |= rs << I_RS_OFFSET;
    binaryInstruction |= rt << I_RT_OFFSET;
    binaryInstruction |= add << I_IMMED_OFFSET;
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}
int instructionILoad(inst *instruction, node *node, char *buf, Symbol *symbolTable){
    unsigned long  binaryInstruction = 0;
    int rs,rt, immed, resCode;
    rs = parseRegister(node);
    if (rs < 0)
        return rs;
    node = node->next;
    resCode = readImmed(node, &immed);
    if(resCode != IMMED_OK)
        return resCode;
    node = node->next;
    rt = parseRegister(node);
    if (rt < 0)
        return rt;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction += instruction->opcode << I_OP_OFFSET;
    binaryInstruction += rs << I_RS_OFFSET;
    binaryInstruction += rt << I_RT_OFFSET;
    binaryInstruction += immed << I_IMMED_OFFSET;
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}

int instructionIArithmetic(inst *instruction, node *node, char *buf) {
    unsigned long  binaryInstruction = 0;
    int rs,rt, immed, resCode;
    rs = parseRegister(node);
    if (rs < 0)
        return rs;
    node = node->next;
    resCode = readImmed(node, &immed);
    if(resCode)
        return resCode;
    node = node->next;
    rt = parseRegister(node);
    if (rt < 0)
        return rt;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction += instruction->opcode << I_OP_OFFSET;
    binaryInstruction += rs << I_RS_OFFSET;
    binaryInstruction += rt << I_RT_OFFSET;
    binaryInstruction += immed << I_IMMED_OFFSET;
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}

int parseJInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable) {
    switch (instruction->IID) {
        case INSTRUCTION_JMP:
            return instructionJJMP(instruction,node,buf, symbolTable);
        case INSTRUCTION_STOP:
            return instructionJStop(instruction,node,buf);
        default:
            return instructionJ(instruction,node,buf, symbolTable);
    }
}

int instructionJJMP(inst *instruction, node *node, char *buf, Symbol *symbolTable) {
    unsigned long  binaryInstruction = 0;
    int rs_flag = 0, resCode, addr; /* immed is 16 bit singed int*/
    char label[32];
    Symbol *s;
    addr = parseRegister(node);
    if (addr >= 0) /* JMP $register */
        rs_flag = 1;
    else if (addr != OPERAND_NOT_REGISTER){ /* JMP $register but bad register */
        return rs_flag;
    }else {
        resCode = readLabel(node);
        if (resCode)
            return resCode;
        strcpy(label, node->value);
        s = findSymbolInTable(symbolTable, label);
        if (s == NULL)
            return LABEL_DOES_NOT_EXIST;
        if(!((s->attributes & (CODE | EXTERNAL))))
            return LABEL_NOT_CODE;
        addr = s->address;
    }
    node = node->next;
    if (node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << J_OP_OFFSET;
    binaryInstruction |= rs_flag << J_REG_FLAG_OFFSET;
    binaryInstruction |= addr << J_ADDR_OFFSET;
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}

int instructionJStop(inst *instruction, node *node, char *buf){
    unsigned long  binaryInstruction = 0;
    binaryInstruction |= instruction->opcode << J_OP_OFFSET;
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}

int instructionJ(inst *instruction, node *node, char *buf, Symbol *symbolTable) {
    unsigned long  binaryInstruction = 0;
    int  resCode, addr; /* immed is 16 bit singed int*/
    char label[32];
    Symbol *s;
    resCode = readLabel(node);
    if (resCode)
        return resCode;
    s = findSymbolInTable(symbolTable, label);
    if (s == NULL)
        return LABEL_DOES_NOT_EXIST;
    if(!((s->attributes & (DATA | EXTERNAL))))
        return LABEL_NOT_CODE;
    addr = s->address;
    node = node->next;
    if (node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << J_OP_OFFSET;
    binaryInstruction |= 0 << J_REG_FLAG_OFFSET;
    binaryInstruction |= addr << J_ADDR_OFFSET;
    printInstruction(buf, binaryInstruction);
    return LINE_OK;
}

/**
 * recives one parameter from line and tries to parse register id
 * @param str  - the parameter that should contain a register number, can be null in which case a MISSING_ARGUMENT error will be returned
 * @return the register id or the appropriate error code
 */
int parseRegister(node *node) {
    char *str;
    if(node == NULL){
        return MISSING_ARGUMENTS;
    }
    str = node->value;
    if(str == NULL || str[0] == NULL){
        return MISSING_ARGUMENTS;
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
int readImmed(node *node, int *immed){
    char *buf;
    if (node == NULL)
        return MISSING_ARGUMENTS;
    buf = node->value;
    if(buf == NULL || strlen(buf)==0){
        return MISSING_ARGUMENTS;
    }
    long prev = 0, res = 0;
    int sing = 1;
    if(!isdigit(buf[0])){
        switch (buf[0]) {
            case '-':
                sing = -1;
                buf++;
                break;
            case '+':
                sing = 1;
                buf++;
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
        if(res <= prev && prev != 0) /* int overflow (int may be bigger then 16 bits) */
            return IMMED_OUT_OF_RANGE;
        buf++;
    }
    res *= sing;
    if(res <= (-(1 << 16)) || res >= ((1 << 16) - 1)) /* res is 16 bit singed so its range in 2's complimint is: -2^15 <= res <= 2^15-1 */
        return IMMED_OUT_OF_RANGE;
    *immed = to16bit(res);
    return IMMED_OK;
}
int readLabel(node *node){
    char *in;
    if(node == NULL)
        return MISSING_ARGUMENTS;
    in = node->value;
    int count = 1;
    if(!isalpha(in[0]))
        return INVALID_LABEL;

    while (in[count] != NULL){
        if(!isalnum(in[count]))
            return INVALID_LABEL;
        count++;
    }
    return count > 31;
}

/**
 * parses the internal instuction buffer into buf with the required format
 * @param buf - outup string buffer
 */
void printInstruction(char *buf,unsigned int binaryInstruction) {
    unsigned char parsedInstruction[4];
    parsedInstruction[0] = (binaryInstruction) & 0b11111111; /* extructing bytes from instruction to "memory" in little endian */
    parsedInstruction[1] = (binaryInstruction >> 8) & 0b11111111;
    parsedInstruction[2] = (binaryInstruction >> 16) & 0b11111111;
    parsedInstruction[3] = (binaryInstruction >> 24) & 0b11111111;
    sprintf(buf, "%.2X %.2X %.2X %.2X", parsedInstruction[0], parsedInstruction[1], parsedInstruction[2], parsedInstruction[3]);
}
/**
 * convert int to 16 bit 2's compliment.
 * we cant guarantee int is 16 bit exactly and uses 2's compliment (even tho it practicaly is)
 * range check is already done
 * @param in
 * @return
 */
int to16bit(int in){
    int sing, out = 0;
    if(in >= 0)
        return in;
    sing = -1;
    in *= sing;
    out = (1 << 16) - in;
    return out;
}