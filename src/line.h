//
// Created by alon on 7/1/21.
//

#ifndef WTFPROJECT_LINE_H
#define WTFPROJECT_LINE_H
#define LINE_LENGTH 80

typedef struct node {
    char *value;
    struct node *next;
} node;

typedef struct line {
    char *label;
    node head;
} line;

line strToLine(char *str);
#endif //WTFPROJECT_LINE_H
