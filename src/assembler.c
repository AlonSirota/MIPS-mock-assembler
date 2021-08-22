//
// Created by alon on 6/27/21.
//
#include "assembler.h"

/**
 * Runs the assembly process on file in fileName
 * @param fileName, path to a .as file to be assembled
 */
void assemblePath(char *fileName) {
    /* Check that fileName ends with .asm */
    char *extension = strstr(fileName, ASSEMBLY_EXTENSION);
    if (extension == NULL) {
        printf("%s is not a valid assembly file name\n", fileName);
        return;
    }

    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error while opening file %s\n", fileName);
        return;
    }

    *extension = '\0'; /* Trim .asm extension */
    assembleFile(f, fileName);
    fclose(f);
}

/**
 * Assembles asFile
 * asFile won't be closed as part of this function.
 * @param asFile parameter is expected to be an opened file.
 * @param fileName
 */
void assembleFile(FILE *asFile, char *fileName) {
    assert(asFile != NULL);
    bytesNode *dataImage;
    int ic, dc;
    FILE *objFile;
    char objFileName[MAX_FILE_NAME_LEN];
    Symbol *symbolTable;
    externalTable  *externalTable = NULL;
    int hasErrors = FALSE;

    if (firstPass(asFile, &ic, &dc, &dataImage, &symbolTable) == GOOD) {
        /* objFileName = "<.as file name>.ob" */
        strcpy(objFileName, fileName);
        strcat(objFileName, ".ob");

        /* object file is opened outside of secondPass because it's simpler to create and remove it if needed from here */
        if ((objFile = fopen(objFileName, "wb"))) {
            /* Generate the different output files, turn on hasErrors flag if encountered any errors in them */
            hasErrors |= (GOOD != writeObjFileHeader(objFile, ic, dc));
            hasErrors |= (GOOD != secondPass(asFile, objFile, symbolTable, &externalTable));
            hasErrors |= (GOOD != appendDataImageToFile(objFile, dataImage, ic));
            fclose(objFile); /* secondPass is done processing object file */

            if (!hasErrors) { /* TODO: delete files when failed */
                hasErrors |= (GOOD != generateEntriesFile(fileName, symbolTable));
                hasErrors |= (GOOD != generateExternalsFile(fileName, externalTable));
            }
        } else {
            printf("Error while opening file %s\n", fileName);
            hasErrors = TRUE;
        }
        printf("ok?");
    }

    if (hasErrors) {
        /* assembleFile function is responsible for deleting objectfile if encountered any errors during assembly process */
        remove(objFileName);
    }
}

/**
 * generate entries file.
 *
 * @param fileName - base filename
 * @param symbolTable - symbol table
 * @return GOOD on success, else the errorCode
 */
enum ErrorCode generateEntriesFile(char *fileName, Symbol *symbolTable){
    char entrFileName[MAX_FILE_NAME_LEN];
    strcpy(entrFileName, fileName);
    strcat(entrFileName, ".ent"); /* basefilename.ent */
    FILE *entrFile;
    if ((entrFile = fopen(entrFileName, "w"))) { /* create new file */
        while (symbolTable != NULL){
            if(symbolTable->attributes & ENTRY){ /* if symbol is entry */
                if(fprintf(entrFile, "%s %.4d\n", symbolTable->label, symbolTable->address) <= 0){ /* fprintf failed*/
                    fclose(entrFile);
                    return FILE_WRITE_ERROR;
                }
            }
            symbolTable = symbolTable->next;
        }
        fclose(entrFile);
        return GOOD;
    }else {
        remove(entrFileName);
        return FILE_WRITE_ERROR;
    }
}

/**
 * generate externals file.
 *
 * @param fileName - base filename
 * @param symbolTable - symbol table
 * @return GOOD on success, else the errorCode
 */
enum ErrorCode generateExternalsFile (char *fileName, externalTable *et){
    char externalFileName[MAX_FILE_NAME_LEN];
    strcpy(externalFileName, fileName);
    strcat(externalFileName, ".ext"); /* basefilename.ext */
    FILE *entrFile;
    if ((entrFile = fopen(externalFileName, "w"))) {
        while (et != NULL){
            if(fprintf(entrFile, "%s %.4d\n", et->label, et->address) <= 0){ /*fprintf failed*/
                fclose(entrFile);
                return FILE_WRITE_ERROR;
            }
            et = et->next;
        }
        fclose(entrFile);
        return GOOD;
    }else {
        remove(externalFileName);
        return FILE_WRITE_ERROR;
    }
}

/*
 * write ic and dc to obj file
 */
enum ErrorCode writeObjFileHeader(FILE *pIobuf, int ic, int dc) {
    /* To get the byte count of the code segment, need to subtract the first memory address from the last */
    return (fprintf(pIobuf, "%d %d\n", ic - FIRST_MEMORY_ADDRESS, dc) > 0 )? GOOD: FILE_WRITE_ERROR;
}

enum ErrorCode firstPass(FILE *asFile, int *icOut, int *dcOut, bytesNode **dataImagePtr, Symbol **symbolTableOut) {
    int lineNumber;
    char lineStr[LINE_LENGTH + 1];
    line lineParsed;
    *symbolTableOut = NULL;
    *dataImagePtr = NULL;
    byteArray directiveBytes;
    int hasErrors = FALSE;
    enum ErrorCode error = GOOD; /* If encountered error */
    Symbol *temp;
    *icOut = FIRST_MEMORY_ADDRESS, *dcOut = 0;
    assert(asFile != NULL);

    /* Iterate over file's lines, while counting the line number */
    /* fgetshred reads a line from asFile to lineStr */
    for (lineNumber = 1; fgetsShred(asFile, LINE_LENGTH + 1, lineStr); lineNumber++) {
        lineParsed = strToLine(lineStr);

        if (!isLineRelevant(lineParsed)) { /* skip irrelevant lines */
            continue;
        }
        else if (isLineDirective(lineParsed)) { /* process directive lines */
            if (lineParsed.label != NULL) { /* if has label, add to symbol table */
                error = addSymbol(symbolTableOut, lineParsed.label, *dcOut, DATA);
                logError(error, &hasErrors, lineNumber);
            }

            /* Encode directive data */
            directiveBytes = directiveToBytes(lineParsed, &error);
            logError(error, &hasErrors, lineNumber);

            if (error == GOOD)
            {
                error = addBytesToImage(dataImagePtr, directiveBytes);
                logError(error, &hasErrors, lineNumber);
                *dcOut += directiveBytes.size;
            }
        }
        else if (!strcmp(lineParsed.head.value,ENTRY_MNEMONIC)) { /* process entry lines */
            error = addSymbol(symbolTableOut, lineParsed.head.next->value, -1, ENTRY);
            logError(error, &hasErrors, lineNumber);
        }
        else if (!strcmp(lineParsed.head.value,EXTERN_MNEMONIC)) { /* process extern lines */
            error = processExtern(lineParsed.head.next, symbolTableOut);
            logError(error, &hasErrors, lineNumber);
        }
        else if (lineParsed.head.value != NULL ) {/* Treat this line as an instruction, as concluded by process of elimination. */
            if(lineParsed.label != NULL){
                error = addSymbol(symbolTableOut, lineParsed.label, *icOut, CODE);
                logError(error, &hasErrors, lineNumber);
            }
            *icOut += 4;
            }
            /* Further processing of instruction line is done in second pass. This deviates then the algorithm in assignment details */
    }
    temp = *symbolTableOut;
    while(temp != NULL){
        if(temp->attributes & DATA)
            temp->address += *icOut;
        temp = temp->next;
    }

    return error;
}

/*
 * Prints an error message like this:
 * also sets hasErrors to true if errorCode is an actual error.
 * "3: message\n"
 */
void logError(enum ErrorCode error, int *hasErrors, int lineNumber) {
    const char* errorMsg;
    if (error != GOOD) {
        errorMsg = codeToMsg(error);
        printf("%d: %s\n", lineNumber, errorMsg);

        *hasErrors = TRUE;
    }
}

/*
 * perfomes a second pass on input file
 * parses instructions and generates the code segment of obj file and externals table
 */
enum ErrorCode secondPass(FILE *asFile, FILE *objFile, Symbol *st, externalTable  **externalTable1){
    int ic = FIRST_MEMORY_ADDRESS, lineNo = 1;
    char lineStr[LINE_LENGTH + 1],  buf[80];
    enum ErrorCode ecTemp, ec = GOOD;
    line lineParsed;
    assert(asFile != NULL);

    rewind(asFile);
    while(fgetsShred(asFile, LINE_LENGTH + 1, lineStr) != NULL){ /**read line*/
        lineParsed = strToLine(lineStr); /*parse line*/
        if(lineParsed.head.value == NULL || lineParsed.head.value[0] == '.'){ /* empty line or a directive*/
            lineNo++;
            continue;
        }
        ecTemp = parseInstruction(&lineParsed.head, buf, st, ic, externalTable1); /*actually parse the instruction and generate code and externals table*/
        if(ecTemp != GOOD){ /* report error */
            printError(ecTemp, lineNo); /*print error*/
            ec = GENERIC_ERROR; /*tell caller function it failed (details not required cuz error output was generated)*/
        }
        if(ec == GOOD){ /*line is good so print code in required format*/
            printLineToFile(objFile, ic, buf);
        }
        ic += 4; /* if there is any error no output file will be generated so no need to worry if necessary to increase ic in case of a bad line */
        lineNo++;
    }
    return ec;
}
/*
 * generate output string for printing instruction line
 * easier to debuf buffers then files....
 * */
void printLineToBuffer(char *out, int ic, char *buf) {
    sprintf(out, "%.4d %s\n", ic, buf);
}
/*
 * print the instruction string generated in secondpass in the required format
 */
enum ErrorCode printLineToFile(FILE *pIobuf, int ic, char *buf) {
    char temp[80];
    printLineToBuffer(temp, ic, buf);
    return (fprintf(pIobuf, "%s", temp) > 0 )? GOOD: FILE_WRITE_ERROR;
}

/*
 * generate error output string
 * easier for testing
 * */
void makeErrStr(char *buff, enum ErrorCode ec, int lineNo){
    char *errorMsg = codeToMsg(ec);
    if (errorMsg == NULL)
        errorMsg = "Unknown error - errorcode was defined in enume ErroeCode but was not given an errorString";
    sprintf(buff, "Error at line %d: %s\n", lineNo, errorMsg);
}
/*
 * print an error
 */
void printError(enum ErrorCode ec, int lineNo){
    char buff[ERROR_BUFFER_LENGTH];
    makeErrStr(buff, ec, lineNo);
    printf("%s", buff);
}
enum ErrorCode addBytesToImage(bytesNode **tablePtr, byteArray bytes) {
    bytesNode * curr;
    bytesNode *next = (bytesNode *) malloc(sizeof (bytesNode)); /* Prepare new symbol */
    if(next == NULL)
        return INSUFFICIENT_MEMORY;
    next->next = NULL;
    next->bytes = bytes;

    if (*tablePtr == NULL) { /* if table is empty */
        *tablePtr = next; /* it now contains the new element. */
        return GOOD;
    }

    /* else, find last element*/
    curr = *tablePtr;
    while (curr->next != NULL)
        curr = curr->next;

    /* append */
    curr->next = next;
    return GOOD;
}

/*
 * Adds extern to symbol table, with address of 0.
 * Returns ErrorCode corresponding to outcome.
 */
enum ErrorCode processExtern(node *operandHead, Symbol **symbolTablePtr) {
    if (operandHead == NULL) {
        return MISSING_OPERAND;
    } else if (!isLastNode(*operandHead)) { /* Should only have this one argument */
        return TOO_MANY_ARGUMENTS;
    } else if (isValidLabel(operandHead->value) == GOOD) {
        return addSymbol(symbolTablePtr, operandHead->value, 0, EXTERNAL);
    } else {
        return INVALID_LABEL;
    }
}

/*
 * Returns true if this line is relevant (and should be processed).
 * Otherwise it's a comment or empty.
 */
int isLineRelevant(line l) {
    /* if mnemonic is empty */
    return !(l.head.value == NULL || !strcmp(l.head.value, ""));
}

/*
 * Appends bytes from dataImage to objFile (which is expected to be opened and writable)
 * returs GOOD or FILE_WRITE_ERROR if failed
 */
enum ErrorCode appendDataImageToFile(FILE *objFile, bytesNode *dataImage, int ic) {
    byteArray bytes;
    bytesNode *curr;
    int currAddr = ic /* address of last appended byteArray */
            ,i;

    /* Overwrite the last character, because it will always be a '\n' char, to prevent consecutive newlines */
    fseek(objFile, -1, SEEK_CUR);

    /* each node */
    for (curr = dataImage; curr != NULL; curr = curr->next) {
        /* each byteArray in node */
        bytes = curr->bytes;
        for (i = 0; i < bytes.size; i++) {
            /* Every few bytes, carriage return and print address */
            if (currAddr % BYTES_PER_LINE == 0) {
                if (fprintf(objFile,"\n%.4d", currAddr) < 0) {
                    return FILE_WRITE_ERROR;
                }
            }

            /* Print current byteArray as two Hex characters
             * cast prevents sign extension causing more than 2 characters to be written */
            if (fprintf(objFile, " %.2X", (unsigned char) bytes.arr[i]) < 0) {
                return FILE_WRITE_ERROR;
            }
            currAddr++;
        }
    }

    return GOOD;
}
