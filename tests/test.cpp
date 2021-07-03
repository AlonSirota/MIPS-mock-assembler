#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "./fmemopen_windows.c"
#endif
#include "gtest/gtest.h"
#include "assembler.c"
#include "line.c"

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