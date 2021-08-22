/*
// Created by Ram Ben on 7/2/2021.
*/
#include "instructionList.h"

#define REGISTER_KEYWORD '$'
#define IMMED_BIT_SIZE 16
inst INSTRUCTIONS[] = {{"add",  'R', 1, 0,  INSTRUCTION_ADD},
                       {"sub",  'R', 2, 0,  INSTRUCTION_SUB},
                       {"and",  'R', 3, 0,  INSTRUCTION_AND},
                       {"or",   'R', 4, 0,  INSTRUCTION_OR},
                       {"nor",  'R', 5, 0,  INSTRUCTION_NOR},
                       {"move", 'R', 1, 1,  INSTRUCTION_MOVE},
                       {"mvhi", 'R', 2, 1,  INSTRUCTION_MVHI},
                       {"mvlo", 'R', 3, 1,  INSTRUCTION_MVLO},
                       {"addi", 'I', 0, 10, INSTRUCTION_ADDI},
                       {"subi", 'I', 0, 11, INSTRUCTION_SUBI},
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

/**
 * search for an instruction
 * @param name - name of an instruction, e.g: "add", "sub"...
 * @return a pointer to the apropriate instruction, null if not found
 */
inst *strToInstruction(char *name){
    inst *ptr = INSTRUCTIONS;
    while (ptr->IID != INSTRUCTION_LIST_END){
        if (strcmp(name, ptr->name) == 0)
            return ptr;
        ptr++;
    }
    return NULL;
}

/**
 * parses an instuction line
 * @param node - the first element in a line
 * @param buf - string buffer to write instruction in hex format
 * @param symbolTable  - lable symbolNode table
 * @param ic - the instruction counter
 * @return the parsing result: GOOD if line is gramatically correct, else returns the error in the line
 */
enum ErrorCode parseInstruction(node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1) {
    inst *instruction;
    if(node == NULL)
        return GOOD; /* empty line do nothing */
    instruction = strToInstruction(node->value);
    if(instruction == NULL)
        return UNRECOGNIZED_INSTRUCTION;
    switch (instruction->type) {
        case 'R':
            return parseRInstruction(instruction, node->next, buf);
        case 'I':
            return parseIInstruction(instruction, node->next, buf, symbolTable, ic);
        case 'J':
            return parseJInstruction(instruction, node->next, buf, symbolTable, ic, externalTable1);
        default:
            return GENERIC_ERROR;
            break; /* impossible.... */
    }
}

/**
 * parse spesifically R typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode parseRInstruction(inst *instruction, node *node, char *buf) {
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

/**
 * parse spesifically R-arithmetic typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionRArithmetic(inst *instruction, node *node, char *buf) {
    unsigned long  binaryInstruction = 0; /* the resulting binary code of the instruction */
    int rs,rd,rt; /* registers */
    enum ErrorCode ec;
    ec = parseRegister(node, &rs);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = parseRegister(node, &rt);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = parseRegister(node, &rd);
    if (ec != GOOD)
        return ec;
    node = node->next;
    if(node != NULL) /* didnt reach end of line */
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << R_OP_OFFSET; /* shifting parameters to the defined position */
    binaryInstruction |= rs << R_RS_OFFSET;
    binaryInstruction |= rt << R_RT_OFFSET;
    binaryInstruction |= rd << R_RD_OFFSET;
    binaryInstruction |= instruction->funct << R_FUNCT_OFFSET;
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * parse spesifically R-Move typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionRMove(inst *instruction, node *node, char *buf) { /* move rd, rs */
    unsigned long  binaryInstruction = 0;
    int rs,rd,rt;
    enum ErrorCode ec;
    ec = parseRegister(node, &rs);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = parseRegister(node, &rd);
    if (ec != GOOD)
        return ec;
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
    return GOOD;
}

/**
 * parse spesifically I typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @param symbolTable - symbol table
 * @param ic = instruction counter
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode parseIInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic) {
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
            return instructionILoad(instruction,node,buf);


    }
}

/**
 * parse spesifically I-Branching typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @param symbolTable - symbol table
 * @param ic = instruction counter
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionIBranch(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic) {
    unsigned long  binaryInstruction = 0;
    int rs,rt, add;
    enum ErrorCode ec;
    char *label;
    Symbol *s;
    ec = parseRegister(node, &rs);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = parseRegister(node, &rt);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = readLabel(node);
    if(ec != GOOD)
        return ec;
    label = node->value;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    s = findSymbolInTable(symbolTable, label);
    if(s == NULL)
        return LABEL_DOES_NOT_EXIST;
    /* see: https://opal.openu.ac.il/mod/ouilforum/discuss.php?d=2967783&p=7079705#p7079705 */
    if((s->attributes & EXTERNAL)) /* cannot branch to external file */
        return EXTERNAL_LABEL;
    add = s->address - ic;
    binaryInstruction |= instruction->opcode << I_OP_OFFSET;
    binaryInstruction |= rs << I_RS_OFFSET;
    binaryInstruction |= rt << I_RT_OFFSET;
    binaryInstruction |= (add & ((1<<16)-1)) << I_IMMED_OFFSET;
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * parse spesifically I-Load typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionILoad(inst *instruction, node *node, char *buf){
    unsigned long  binaryInstruction = 0;
    int rs,rt, immed;
    enum ErrorCode ec;
    ec = parseRegister(node, &rs);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = readImmed(node, &immed);
    if(ec != GOOD)
        return ec;
    node = node->next;
    ec = parseRegister(node, &rt);
    if (ec != GOOD)
        return ec;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction += instruction->opcode << I_OP_OFFSET;
    binaryInstruction += rs << I_RS_OFFSET;
    binaryInstruction += rt << I_RT_OFFSET;
    binaryInstruction += (immed & ((1<<IMMED_BIT_SIZE)-1)) << I_IMMED_OFFSET; /* int may be >16bit so trimming for negative values */
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * parse spesifically I-Arithmetic typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionIArithmetic(inst *instruction, node *node, char *buf) {
    unsigned long  binaryInstruction = 0;
    int rs,rt, immed;
    enum ErrorCode ec;
    ec = parseRegister(node, &rs);
    if (ec != GOOD)
        return ec;
    node = node->next;
    ec = readImmed(node, &immed);
    if(ec != GOOD)
        return ec;
    node = node->next;
    ec = parseRegister(node, &rt);
    if (ec != GOOD)
        return ec;
    node = node->next;
    if(node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction += instruction->opcode << I_OP_OFFSET;
    binaryInstruction += rs << I_RS_OFFSET;
    binaryInstruction += rt << I_RT_OFFSET;
    binaryInstruction += (immed & ((1<<IMMED_BIT_SIZE)-1)) << I_IMMED_OFFSET;
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * parse spesifically J typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @param symbolTable - symbole table
 * @param ic = instruction counter
 * @param externalTable1 - pointer to external table
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode parseJInstruction(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1) {
    switch (instruction->IID) {
        case INSTRUCTION_JMP:
            return instructionJJMP(instruction,node,buf, symbolTable,ic, externalTable1);
        case INSTRUCTION_STOP:
            return instructionJStop(instruction,node,buf);
        default:
            return instructionJ(instruction,node,buf, symbolTable, ic, externalTable1);
    }
}

/**
 * parse spesifically J typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @param symbolTable - symbole table
 * @param ic = instruction counter
 * @param externalTable1 - pointer to external table
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionJJMP(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1) {
    unsigned long  binaryInstruction = 0;
    int rs_flag = 0, addr; /* immed is 16 bit singed int*/
    enum ErrorCode ec;
    char label[32];
    Symbol *s;
    ec = parseRegister(node, &addr);
    if (ec == GOOD){ /* JMP $register */
        rs_flag = 1;
    }
    else if (ec == MISSING_ARGUMENTS || ec == OPERAND_NOT_VALID_REGISTER){ /* JMP $register but bad register */
        return ec;
    }else { /* jmp label */
        ec = readLabel(node);
        if (ec != GOOD)
            return ec;
        strcpy(label, node->value);
        s = findSymbolInTable(symbolTable, label);
        if (s == NULL)
            return LABEL_DOES_NOT_EXIST;

        if(((s->attributes & EXTERNAL))) /* if used external label save where */
            addExternal(externalTable1, s->label, ic);

        addr = s->address;
    }
    node = node->next;
    if (node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << J_OP_OFFSET;
    binaryInstruction |= rs_flag << J_REG_FLAG_OFFSET;
    binaryInstruction |= addr << J_ADDR_OFFSET;
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * parse spesifically the stop instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionJStop(inst *instruction, node *node, char *buf){
    unsigned long  binaryInstruction = 0;
    if (node != NULL) /* no parameters */
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << J_OP_OFFSET;
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * parse generic J typed instruction
 * parsing is grouped by similar types of INSTRUCTIONS
 * @param instruction - instruction type
 * @param node - first relevant line parameter
 * @param buf - string output buffer
 * @param symbolTable - symbole table
 * @param ic = instruction counter
 * @param externalTable1 - pointer to external table
 * @return GOOD on success, else the error that occurred
 */
enum ErrorCode instructionJ(inst *instruction, node *node, char *buf, Symbol *symbolTable, int ic, externalTable  **externalTable1) {
    unsigned long  binaryInstruction = 0;
    int  addr; /* immed is 16 bit singed int*/
    enum ErrorCode ec;
    char label[32];
    Symbol *s;
    ec = readLabel(node);
    if (ec != GOOD)
        return ec;
    strcpy(label, node->value);
    s = findSymbolInTable(symbolTable, label);
    if (s == NULL)
        return LABEL_DOES_NOT_EXIST;
    if(s->attributes &  EXTERNAL)
        addExternal(externalTable1, s->label, ic);
    addr = s->address; /* should be valid */
    node = node->next;
    if (node != NULL)
        return TOO_MANY_ARGUMENTS;
    binaryInstruction |= instruction->opcode << J_OP_OFFSET;
    binaryInstruction |= 0 << J_REG_FLAG_OFFSET;
    binaryInstruction |= addr << J_ADDR_OFFSET;
    printInstruction(buf, binaryInstruction);
    return GOOD;
}

/**
 * recives one parameter from line and tries to parse register id
 * @param str  - the parameter that should contain a register number, can be null in which case a MISSING_ARGUMENT error will be returned
 * @return the register id or the appropriate error code
 */
enum ErrorCode parseRegister(node *node, int *reg) {
    char *str;
    int num;
    if(node == NULL){
        return MISSING_ARGUMENTS;
    }
    str = node->value;
    if(str == NULL || str[0] == 0){ /* null string or zero length string */
        return MISSING_ARGUMENTS;
    }
    if(str[0] != REGISTER_KEYWORD) /* start with $*/
        return OPERAND_NOT_REGISTER;
    if(!isdigit(str[1]))
        return OPERAND_NOT_VALID_REGISTER;
    num = atoi(str + 1);
    if(num <0 || num > 31)
        return OPERAND_NOT_VALID_REGISTER;
    *reg = num;
    return GOOD;
}

/**
 * recives one parameter from line and tries to parse a valid immediate
 * @param buf  - the parameter that should contain a valid immediate, can be null in which case a MISSING_ARGUMENT error will be returned
 * @return the value of the immediate or the appropriate error code
 */
enum ErrorCode readImmed(node *node, int *immed){
    char *buf;
    long res;
    if (node == NULL)
        return MISSING_ARGUMENTS;
    buf = node->value;
    if(buf == NULL || strlen(buf)==0){  /* null string or zero length string */
        return MISSING_ARGUMENTS;
    }
    res = 0;
    if (parseLong(node->value, &res) == FALSE){
        return ILLEAGLE_IMMED;
    }
    if(res <= (-(1 << IMMED_BIT_SIZE)) || res >= ((1 << IMMED_BIT_SIZE) - 1)){ /* res is 16 bit singed so its range in 2's complimint is: -2^15 <= res <= 2^15-1 */
        return IMMED_OUT_OF_RANGE;
    }
    *immed = to16bit(res);
    return GOOD;
}

/**
 * recives one parameter from line and does BASIC label validation.
 * even if label isnt valid for other reason it would still result in label doesnt exist error (first pass does comprehensive label validation
 * @param buf  - the parameter that should contain a valid immediate, can be null in which case a MISSING_ARGUMENT error will be returned
 * @return the value of the immediate or the appropriate error code
 */
enum ErrorCode readLabel(node *node){
    char *in;
    int count;
    if(node == NULL)
        return MISSING_ARGUMENTS;
    in = node->value;
    count = 1;
    if(!isalpha(in[0]))
        return INVALID_LABEL;

    while (in[count] != 0){
        if(!isalnum(in[count]))
            return INVALID_LABEL;
        count++;
    }
    return (count <= 31)?GOOD:INVALID_LABEL;
}

/**
 * parses the internal instuction buffer into buf with the required format
 * @param buf - outup string buffer
 */
void printInstruction(char *buf,unsigned int binaryInstruction) {
    unsigned char parsedInstruction[4];
    parsedInstruction[0] = (binaryInstruction) & ONE_BYTE_CONST; /* extructing bytes from instruction to "memory" in little endian */
    parsedInstruction[1] = (binaryInstruction >> 8) & ONE_BYTE_CONST;
    parsedInstruction[2] = (binaryInstruction >> 16) & ONE_BYTE_CONST;
    parsedInstruction[3] = (binaryInstruction >> 24) & ONE_BYTE_CONST;
    sprintf(buf, "%.2X %.2X %.2X %.2X", parsedInstruction[0], parsedInstruction[1], parsedInstruction[2], parsedInstruction[3]);
}
/**
 * convert int to 16 bit 2's compliment.
 * we cant guarantee int is 16 bit exactly and uses 2's compliment (even tho it practicaly is)
 * range check is already done
 * @param in number
 * @return calculate 2's compliment
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

