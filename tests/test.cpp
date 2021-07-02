#include "gtest/gtest.h"
#include "assembler.c"
#include "line.c"

TEST(fgetsShred, brolo) {
    FILE *stream;
    char buffer[] = "a\nb";
    char out[40];
    stream = fmemopen(buffer, strlen(buffer), "r");
    fgetsShred(stream, 2+1, out);
    ASSERT_STREQ("a", out) << "Error in reading from stream";
    ASSERT_EQ('b', fgetc(stream)) << "Shred even though reaching \n";
}

TEST(fgetsShred, brolo2) {
    FILE *stream;
    char buffer[] = "ab";
    char out[40];
    stream = fmemopen(buffer, strlen(buffer), "r");
    fgetsShred(stream, 1+1, out);
    ASSERT_STREQ("a", out) << "Error in reading from stream";
    ASSERT_EQ(EOF, fgetc(stream)) << "Didn't shred";
}

TEST(strToLine, test) {
    char str[] = "label: a b,c";
    line l = strToLine(str);
    ASSERT_STREQ(l.label, "label") << "label error";
    ASSERT_STREQ(l.head.value, "a") << "mnemonic error";
    ASSERT_STREQ(l.head.next->value, "b") << "first parameter error";
    ASSERT_STREQ(l.head.next->next->value, "c") << "second parameter error";
    //TODO DESTROY LINE
}