#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

bool compareFiles(const std::string& expected, const std::string& actual) {
    char *expectedLine = static_cast<char *>(calloc(sizeof(char), 100));
    char *actualLine = static_cast<char *>(calloc(sizeof(char), 100));
    FILE *expectedFile = fopen(expected.c_str(), "r"),
    *actualFile = fopen(actual.c_str(), "a+");
    fseek(actualFile, -1, SEEK_END);
    char c;
    if ((c = getc(actualFile)) == '\n') {
        unsigned long len = ftell(actualFile);
        ftruncate(fileno(actualFile), len - 1);
    }

    fseek(actualFile, 0, SEEK_SET);
    while (fgets(expectedLine, 99, expectedFile) && fgets(actualLine, 99, actualFile)) {
        if (strcmp(expectedLine, actualLine)) {
            printf("Expected: %s\nActual: %s\n",expectedLine, actualLine);
            return false;
        }
    }
    if (getc(expectedFile) == EOF) {
        fgets(actualLine, 99, actualFile);
        if ((!strcmp(actualLine, "\n")) || fgetc(actualFile) != EOF){
            return false;
        }
    }

    return true;
    // TODO fix memory leak here, but low priority.
}
