#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "./fmemopen_windows.c"
#endif
#include "gtest/gtest.h"
#include "assembler.c"
#include "line.c"
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
    ASSERT_EQ(status, LINE_OK);
    ASSERT_EQ(strcmp(output,buf), 0);
}