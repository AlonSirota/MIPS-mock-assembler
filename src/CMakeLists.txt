cmake_minimum_required(VERSION 3.19)
project(assembler C)

set(CMAKE_C_STANDARD 11)

add_executable(assembler
        assembler.c
        assembler.h
        main.c
        line.c
        line.h
        symbolTable.c
        symbolTable.h
        instructionList.c
        instructionList.h
        directive.c
        directive.h consts.h consts.c externalTable.c externalTable.h helper.c helper.h)
