main: main.c assembler.o externalTable.o line.o consts.o helper.o directive.o symbolTable.o instructionList.o
	gcc  -ansi -Wall -pedantic main.c assembler.o externalTable.o line.o consts.o helper.o directive.o symbolTable.o instructionList.o -o assembler
assembler.o: assembler.c assembler.h consts.o line.o helper.o directive.o symbolTable.o externalTable.o
	gcc  -ansi -Wall -pedantic -c assembler.c line.c consts.c helper.c directive.c symbolTable.c externalTable.c 
line.o: line.c line.h 
	gcc  -ansi -Wall -pedantic -c line.c consts.c 
consts.o: consts.c consts.h
	gcc  -ansi -Wall -pedantic -c consts.c 
helper.o: helper.c helper.h
	gcc  -ansi -Wall -pedantic -c helper.c consts.c 
directive.o: directive.c directive.h
	gcc  -ansi -Wall -pedantic -c directive.c directive.h line.c
symbolTable.o: symbolTable.c symbolTable.h instructionList.o
	gcc  -ansi -Wall -pedantic -c symbolTable.c instructionList.c helper.c consts.c
instructionList.o: instructionList.c instructionList.h externalTable.o
	gcc  -ansi -Wall -pedantic -c instructionList.c directive.c externalTable.c symbolTable.c 
externalTable.o: externalTable.c externalTable.h
	gcc  -ansi -Wall -pedantic -c externalTable.c consts.c 
clean:
	rm *.o a.out
