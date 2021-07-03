#define USING_WINDOWS
#include "gtest/gtest.h"
#include "assembler.c"
#include "line.c"
#include "./fmemopen_windows.c"
#include "instructionList.c"

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
    //TODO DESTROY LINE
}

TEST(strToLine, noLabel) {
    char str[] = "a b,c";
    line l = strToLine(str);
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "c") << "post empty parameter error";
    //TODO DESTROY LINE
}

TEST(strToLine, emptyParameters) {
    char str[] = "label: a b,, c";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "") << "empty parameter error";
    ASSERT_STREQ(l.head.next->next->next->value, "c") << "post empty parameter error";
    //TODO DESTROY LINE
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
    char output[] = {0, 101, 72, 64}; /* see page 27 first intruction in table*/
    char codeSeg[4];
    int status, ic = 100;
    char str[] = "add $3,$5,$9";
    line l = strToLine(str);
    inst *instruction = findInstruction(l.head.value);
    status = parseRInstruction(instruction, l.head.next, &ic, codeSeg);
    ASSERT_EQ(status, LINE_OK);
    ASSERT_NE(memcmp(output,codeSeg,4), 0);
}