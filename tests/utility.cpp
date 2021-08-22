#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

bool compareFiles(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        fprintf(stderr, "Failed to open a file\n");
        return false; //file problem
    }
    /*
    if (f1.tellg() != f2.tellg()) {
        printf("Files are of different lengths\n");
        return false; //size mismatch
    }
*/
    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);

    char c1;
    char c2;

    while (!f1.eof() && !f2.eof()){
        while (!f1.eof() && (c1 = f1.get()) != '\r');
        while (!f2.eof() && (c2 = f2.get()) != '\r');
        if(c1 != c2){
            while (!f1.eof() && ((c1 = f1.get()) != '\r' || c1 != '\n'));
            while (!f2.eof() && ((c2 = f2.get()) != '\r' || c2 != '\n'));
            if(f1.eof() && f2.eof())
                return true;
            return false;
        }
    }
    return true;

    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

