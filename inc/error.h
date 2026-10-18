// <error.h>::
#pragma once

#include <stdio.h>

enum {
    SYM_OVERFLOW,
    MATH_DIV_BY_ZERO,
    LEX_PREMATURE_EOF,
    PARS_TOKEN_MISMATCH,
};

extern FILE *outerror;

extern int err;
extern long errline;
extern int errtk;
extern int expectedtk;

extern void printErr();
