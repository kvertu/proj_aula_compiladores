// <error.h>::
#pragma once

#include <stdio.h>

enum {
    SYM_OVERFLOW = -1024,
    MATH_DIV_BY_ZERO = -1025,
    LEX_PREMATURE_EOF = -1026,
    PARS_TOKEN_MISMATCH = -1027,
};

extern FILE *outerror;

extern int err;
extern long errline;
extern int errtk;
extern int expectedtk;

extern void printErr();
