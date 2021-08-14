#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "./fmemopen_windows.c"
#endif
#include "gtest/gtest.h"
#include "assembler.c"
#include "line.c"
#include "instructionList.c"
#include "directive.c"
#include "symbolTable.c"
#include "consts.c"

TEST(fgetsShred, Shred) {
    FILE *stream;
    char buffer[] = "a\nb";
    char out[40];
    stream = fmemopen(buffer, strlen(buffer), "r");
    fgetsShred(stream, 2+1, out);
    ASSERT_STREQ("a", out) << "Error in reading from stream";
    ASSERT_EQ('b', fgetc(stream)) << "Shred even though reaching \n";
}

TEST(fgetsShred, NoShred) {
    FILE *stream;
    char buffer[] = "ab";
    char out[40];
    stream = fmemopen(buffer, strlen(buffer), "r");
    fgetsShred(stream, 1+1, out);
    ASSERT_STREQ("a", out) << "Error in reading from stream";
    ASSERT_EQ(EOF, fgetc(stream)) << "Didn't shred";
}

TEST(strToLine, easy) {
    char str[] = "label: a b,c";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "c") << "post empty parameter error";
    freeLine(l);
}

TEST(strToLine, asciiz_good_syntax) {
    char str[] = "label: .asciz \" word \" ";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, ".asciz") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, " word ") << "first parameter error";
    freeLine(l);
}

TEST(strToLine, asciiz_extra_chars) {
    char str[] = "label: .asciz \" word \" bad";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, ".asciz") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, " word ") << "first parameter error";
    ASSERT_EQ(NULL, l.head.next->next) << "Post string input error";
    ASSERT_EQ(l.error, ASCIIZ_EXTRA_TOKENS) << "Extra tokens not detected";
    freeLine(l);
}

TEST(strToLine, asciiz_unbalanced_parenthesis) {
    char str[] = "label: .asciz \" word ";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, ".asciz") << "mnemonic error";
    ASSERT_EQ(NULL, l.head.next) << "first parameter error";
    ASSERT_EQ(l.error,ASCIIZ_UNBALANCED_PARENTHESIS) << "Unbalanced parenthesis error not flagged";
    freeLine(l);
}

TEST(strToLine, asciiz_missing_parenthesis) {
    char str[] = "label: .asciz word";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, ".asciz") << "mnemonic error";
    ASSERT_EQ(NULL, l.head.next) << "first parameter error";
    ASSERT_EQ(l.error,ASCIIZ_MISSING_PARENTHESIS) << "Unbalanced parenthesis error not flagged";
    freeLine(l);
}

TEST(strToLine, emptyLabel) {
    char str[] = ": a b,c";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "") << "label error";
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "c") << "post empty parameter error";
    freeLine(l);
}

TEST(strToLine, noLabel) {
    char str[] = "a b,c";
    line l = strToLine(str);
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "c") << "post empty parameter error";
    freeLine(l);
}

TEST(strToLine, emptyParameters) {
    char str[] = "label: a b,, c";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "") << "empty parameter error";
    ASSERT_STREQ(l.head.next->next->next->value, "c") << "post empty parameter error";
    freeLine(l);
}

TEST(strToLine, emptyLine) {
    char str[] = "";
    line l = strToLine(str);
    ASSERT_TRUE(l.label == NULL) << "label error";
    ASSERT_TRUE(l.head.value == NULL) << "mnemonic error";
    ASSERT_TRUE(l.head.next == NULL) << "first parameter error";
    freeLine(l);
}

TEST(strToLine, whiteSpaceLine) {
    char str[] = " \t";
    line l = strToLine(str);
    ASSERT_TRUE(l.label == NULL) << "label error";
    ASSERT_TRUE(l.head.value == NULL) << "mnemonic error";
    ASSERT_TRUE(l.head.next == NULL) << "first parameter error";
    freeLine(l);
}

TEST(strToLine, commentLine) {
    char str[] = "; label: a b,c";
    line l = strToLine(str);
    ASSERT_TRUE(l.label == NULL) << "label error";
    ASSERT_TRUE(l.head.value == NULL) << "mnemonic error";
    ASSERT_TRUE(l.head.next == NULL) << "first parameter error";
    freeLine(l);
}

TEST(strToLine, singleParameterInst) {
    char str[] = "add $3";
    line l = strToLine(str);
    ASSERT_TRUE(l.label == NULL) << "label error";
    ASSERT_STREQ(l.head.value, "add") << " error";
    ASSERT_FALSE(l.head.next == NULL) << "first parameter error";
    ASSERT_STREQ(l.head.next->value, "$3");
    freeLine(l);
}

TEST(trimWhiteSpace, leading) {
    char str[] = " \t\na \t\n";
    ASSERT_STREQ(firstNoneSpace(str), "a \t\n");
}

TEST(trimWhiteSpace, trailing) {
    char str[] = " \t\na \t\n";
    trimTrailingSpace(str);
    ASSERT_STREQ(str, " \t\na");
}

TEST(trimWhiteSpace, both) {
    char str[] = " \t\na \t\n";
    char *trimmed = trimWhiteSpace(str);
    ASSERT_STREQ(trimmed, "a");
}
TEST(findIntruction, noLable){
    inst *instruction = findInstruction("add");
    ASSERT_EQ(instruction == NULL, 0);
    instruction = findInstruction("lol");
    ASSERT_NE(instruction == NULL, 0);
}
TEST(parseRInstructions, noLabel){
    char buf[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    char str[] = "add $3,$5,$9";
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}

TEST(addSymbol, easy) {
    Symbol *tableHead = NULL; /* aka empty */
    addSymbol(&tableHead, "a", 1, DATA);
    addSymbol(&tableHead, "b", 2, DATA | CODE);

    Symbol *curr = tableHead;
    ASSERT_TRUE(curr != NULL);
    ASSERT_TRUE(!strcmp(curr->label, "a"));
    ASSERT_TRUE(curr->address == 1);
    ASSERT_TRUE(curr->attributes == DATA);
    curr = curr->next;
    ASSERT_TRUE(curr != NULL);
    ASSERT_TRUE(!strcmp(curr->label, "b"));
    ASSERT_TRUE(curr->address == 2);
    ASSERT_TRUE(curr->attributes == DATA | CODE);
    ASSERT_TRUE(curr->next == NULL);
    discardTable(tableHead);
}

TEST(addSymbol, preventDuplicates) {
    Symbol *tableHead = NULL; /* aka empty */
    addSymbol(&tableHead, "a", 1, DATA);
    addSymbol(&tableHead, "a", 2, DATA | CODE);

    Symbol *curr = tableHead;
    ASSERT_TRUE(curr != NULL);
    ASSERT_TRUE(!strcmp(curr->label, "a"));
    ASSERT_TRUE(curr->address == 1);
    ASSERT_TRUE(curr->attributes == DATA);
    ASSERT_TRUE(curr->next == NULL);
    discardTable(tableHead);
}
TEST(parseInstructionRArithmetic, base){
    char buf[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    int status, ic = 100;
    char str[] = "add $3,$5,$9";
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(parseInstructionRArithmetic, tooFewArgs){
    char buf[20];
    char str[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    strcpy(str,"add $3");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"add $3,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str, "add $3,$5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str, "add $3,$5,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str, "add $3,$5,$9,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
}
TEST(parseInstructionRArithmetic, wrongContext){
    char buf[20];
    char str[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    strcpy(str,"add $lol,lol,lol");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str,"add $50,lol,$4");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str, "add $3,$5,lol");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
    strcpy(str, "add lol,$5,lol");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
    strcpy(str, "add $30,$50,$90");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
}
TEST(parseInstructionRMove, base){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "40 20 80 06"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    char str[] = "move $20,$4";
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(parseInstructionRMove, tooFewArgs){
    char buf[20];
    char str[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    strcpy(str,"move $3");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"move $3,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str, "move $3,$5,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
    strcpy(str, "move $3,$5,$9");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
}
TEST(parseInstructionRMove, wrongContext){
    char buf[20];
    char str[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    strcpy(str,"move $300");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str,"move lol,lol");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
    strcpy(str, "move $34,$5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str, "move $3,$54$9");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, buf);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
}

TEST(instructionIBranch, base){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "1C 00 82 48"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[] = "bgt $4,$2,END";
    Symbol *st = NULL;
    addSymbol(&st, "END", 148, CODE);
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(instructionIBranch, tooFewArgs){
    char buf[20];
    char str[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    strcpy(str,"bgt $3");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"bgt $3,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str, "bgt $3,$5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str, "bgt $3,$5,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, INVALID_LABEL);
    strcpy(str, "bgt $3,$5,END,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
}
TEST(instructionIBranch, wrongContext){
    char buf[20];
    char str[20];
    char *output = "40 48 65 00"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    strcpy(str,"bgt $300");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str,"bgt lol,lol");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
    strcpy(str, "bgt $34,$5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str, "bgt $3,$5,$fsd");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, INVALID_LABEL);
}

TEST(instructionILoad, base){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "04 00 0A 58"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[] = "sw $0,4,$10";
    Symbol *st = NULL;
    addSymbol(&st, "END", 148, CODE);
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(instructionILoad, tooFewArgs){
    char buf[20];
    char str[20];
    char *output = "04 00 0A 58"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    strcpy(str,"sw $0");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"sw $0,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"sw $0,4");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"sw $0,4,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"sw $0,4,$10,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
}
TEST(instructionILoad, wrongContext){
    char buf[20];
    char str[20];
    char *output = "04 00 0A 58"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    strcpy(str,"sw $300");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str,"sw lol,lol");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
    strcpy(str, "sw $3,$5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, ILLEAGLE_IMMED);
    strcpy(str, "bgt $3,0,$fsd");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
}
TEST(instructionIBranch, externalLabel){
    char buf[20];
    char *output = "1C 00 82 48"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[] = "bgt $4,$2,END";
    Symbol *st = NULL;
    addSymbol(&st, "END", 148, EXTERNAL);
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, EXTERNAL_LABEL);
}
TEST(instructionIBranch, datalLabel){
    char buf[20];
    char *output = "1C 00 82 48"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[] = "bgt $4,$2,END";
    Symbol *st = NULL;
    addSymbol(&st, "END", 148, DATA);
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, GOOD);
}

TEST(instructionIArithmetic, base){
    char buf[20];
    char *output = "FB FF 22 35"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[] = "ori $9,-5,$2";
    Symbol *st = NULL;
    addSymbol(&st, "END", 148, CODE);
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(instructionIArithmetic, tooFewArgs){
    char buf[20];
    char str[20];
    char *output = "04 00 0A 58"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    strcpy(str,"ori $9");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"ori $9,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"ori $9,-5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"ori $9,-5,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"ori $9,-5,$2,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);

}
TEST(instructionIArithmetic, wrongContext){
    char buf[20];
    char str[20];
    char *output = "04 00 0A 58"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    strcpy(str,"ori $300");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
    strcpy(str,"ori lol,lol");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_REGISTER);
    strcpy(str, "ori $3,$5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, ILLEAGLE_IMMED);
    strcpy(str, "ori $3,0,$fsd");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseIInstruction(instruction, l.head.next, buf, st, ic);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);
}

TEST(instructionJJMP, baseRegister){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "04 00 00 7A"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[] = "jmp $4";
    Symbol *st = NULL;
    addSymbol(&st, "Next", 116, CODE);
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(instructionJJMP, baseLabel){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "74 00 00 78"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[20];
    Symbol *st = NULL;
    addSymbol(&st, "Next", 116, CODE);
    addSymbol(&st, "Last", 116, DATA);
    addSymbol(&st, "First", 116, CODE | EXTERNAL);
    addSymbol(&st, "Prev", 116, CODE | ENTRY);
    strcpy(str,"jmp Next");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"jmp Last");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"jmp First");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"jmp Prev");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}
TEST(instructionJJMP, tooFewArgs){
    char buf[20];
    char str[20];
    char *output = "04 00 00 7A"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    addSymbol(&st, "Next", 116, CODE);
    strcpy(str,"jmp ");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, MISSING_ARGUMENTS);
    strcpy(str,"jmp $9,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
    strcpy(str,"jmp $9,-5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
    strcpy(str,"jmp Next,-5");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
    strcpy(str,"jmp Next,");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);

}
TEST(instructionJJMP, wrongContext){
    char buf[20];
    char str[20];
    char *output = "04 00 00 7A"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    Symbol *st = NULL;
    addSymbol(&st, "Next", 116, CODE);
    strcpy(str,"jmp $900");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, OPERAND_NOT_VALID_REGISTER);

    /* labels are checked before.. */
}
TEST(instructionJStop, baseRegister){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "00 00 00 FC"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[20];
    Symbol *st = NULL;
    strcpy(str,"stop");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"stop $1");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, TOO_MANY_ARGUMENTS);
}
TEST(instructionJ, baseLabel){// wont work.... mistake in the maman....
    char buf[20];
    char *output = "A1 00 00 7C"; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 120;
    char str[20];
    Symbol *st = NULL;
    addSymbol(&st, "A", 161, CODE);
    addSymbol(&st, "K", 161, DATA);
    addSymbol(&st, "B", 161, CODE | EXTERNAL);
    addSymbol(&st, "C", 161, CODE | ENTRY);
    strcpy(str,"la K");
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"la A");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"la B");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
    strcpy(str,"la C");
    l = strToLine(str);
    instruction = findInstruction(l.head.value);
    status = parseJInstruction(instruction, l.head.next, buf, st);
    ASSERT_EQ(status, GOOD);
    ASSERT_EQ(strcmp(output,buf), 0);
}

TEST(directiveToByteTest, db) {
    byte buffer[1000];
    byte *res;
    ErrorCode e;
    node nextNext = {.value = "-12", .next = NULL};
    node next = {.value = "31", .next = &nextNext};
    node head = {.value = ".db", .next = &next};
    line l = {.head = head};
    res = directiveToBytes(l, &e);
    ASSERT_TRUE(res[0] == 0b00011111); /* 32 in binary */
    ASSERT_TRUE(res[1] == 0b11110100); /* -12 in binary */

    free(res);
}
TEST(directiveToByteTest, dw) {
    byte buffer[1000];
    byte *res;
    ErrorCode e;
    node nextNext = {.value = "-12", .next = NULL};
    node next = {.value = "31", .next = &nextNext};
    node head = {.value = ".dw", .next = &next};
    line l = {.head = head};
    res = directiveToBytes(l,&e);

    ASSERT_TRUE(res[0] == 0b00011111); /* 32 in binary */
    ASSERT_TRUE(res[1] == 0);
    ASSERT_TRUE(res[2] == 0);
    ASSERT_TRUE(res[3] == 0);

    ASSERT_TRUE(res[4] == 0b11110100); /* -12 in binary */
    ASSERT_TRUE(res[5] == 0b11111111);
    ASSERT_TRUE(res[6] == 0b11111111);
    ASSERT_TRUE(res[7] == 0b11111111);

    free(res);
}
TEST(directiveToByteTest, dh) {
    byte buffer[1000];
    byte *res;
    ErrorCode e;
    node nextNext = {.value = "-12", .next = NULL};
    node next = {.value = "31", .next = &nextNext};
    node head = {.value = ".dh", .next = &next};
    line l = {.head = head};
    res = directiveToBytes(l,&e);

    ASSERT_TRUE(res[0] == 0b00011111); /* 32 in binary */
    ASSERT_TRUE(res[1] == 0b00000000);

    ASSERT_TRUE(res[2] == 0b11110100); /* -12 in binary */
    ASSERT_TRUE(res[3] == 0b11111111);
    free(res);
}

TEST(printErrors, codeToMsg){
    enum ErrorCode ec = INVALID_LABEL;
    const char *errStr = codeToMsg(ec);
    ASSERT_STREQ(errStr, "Invalid label");
}
TEST(printErrors, printErrorOK){
    enum ErrorCode ec = INVALID_LABEL;
    char buf[256];
    makeErrStr(buf, ec, 100);
    ASSERT_STREQ(buf, "Error at line 100: Invalid label\n");
}