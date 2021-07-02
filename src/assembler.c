//
// Created by alon on 6/27/21.
//
#include "assembler.h"
#include "line.h"

void assemblePath(char *fileName) {
    // TODO enforce '.as' file extension.
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error while opening file %s\n", fileName);
    }
    assembleFile(f);
    fclose(f);
}

/*
 * Assembles file f.
 * f parameter is expected to be an opened file.
 * f won't be closed as part of this function.
 */
void assembleFile(FILE *f) {
    assert(f != NULL);
    firstPass(f);
    // TODO Second pass
}

void firstPass(FILE *f) {
    int ic = 100, dc = 0;
    char lineStr[LINE_LENGTH + 1];
    line lineParsed;
    assert(f != NULL);
    fgetsShred(f, LINE_LENGTH + 1, lineStr);
    lineParsed = strToLine(lineStr);

    /* TODO:
     * Add symbol if exists
     * Adjust ic or dc
     * Do symbol table stuff
     */
}

/*
 * Like fgets, but flushes f till end of line / EOF.
 */
char *fgetsShred(FILE *f, int n, char *buffer) {
    int i, ch, reachedEndOfLine = FALSE;

    ch = getc(f);
    if (n <= 0 || ch == EOF) { /* If nothing to be read, or EOF already reached before reading */
        return NULL;
    }
    ungetc(ch, f); /* Pushback to be read in loop later */
    n--; /* leave space fo NULL */

    for (i = 0; i < n; i++) {
        ch = getc(f);

        if (ch == EOF || ch == '\n') {
            reachedEndOfLine = TRUE;
            break;
        } else {
            buffer[i] = ch;
        }
    }
    buffer[i] = '\0';

    while (!reachedEndOfLine) { /* until reaching end of line */
        /* Shred: Read trailing characters, to avoid reading them later as a new line */
        ch = getc(f);
        if (ch == EOF || ch == '\n') {
            reachedEndOfLine = TRUE;
        }
    }

    return buffer;
}
