// <tokens.h>::
#pragma once

enum {
    ID = 1024,
    NUM,
    ASGN,  //RIGHT ASSOCIATIVE ASSIGNMENT OPERATOR
};

extern void printToken(FILE *out, int tk);
