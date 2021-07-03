//
// Created by Ram Ben on 7/2/2021.
//
#define INSTRUCTION_ADD 1
#define INSTRUCTION_SUB 2
#define INSTRUCTION_AND 3
#define INSTRUCTION_OR 4
#define INSTRUCTION_NOR 5
#define INSTRUCTION_MOVE 6
#define INSTRUCTION_MVHI 7
#define INSTRUCTION_MVLO 8
#define INSTRUCTION_ADDI 9
#define INSTRUCTION_SUBI 10
#define INSTRUCTION_ANDI 11
#define INSTRUCTION_ORI 12
#define INSTRUCTION_NORI 13
#define INSTRUCTION_BNE 14
#define INSTRUCTION_BEQ 15
#define INSTRUCTION_BLT 16
#define INSTRUCTION_BGT 17
#define INSTRUCTION_LB 18
#define INSTRUCTION_SB 19
#define INSTRUCTION_LW 20
#define INSTRUCTION_SW 21
#define INSTRUCTION_LH 22
#define INSTRUCTION_SH 23
#define INSTRUCTION_JMP 24
#define INSTRUCTION_LA 25
#define INSTRUCTION_CALL 26
#define INSTRUCTION_STOP 27


typedef struct inst{
    char *name;
    char type;
    int funct;
    int opcode;
    int UID; /* easier to compere insted of string */
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
                       };
/* TODO: add find instruction in list, return the inst add list of  */