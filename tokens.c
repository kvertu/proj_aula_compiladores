// <tokens.c>::
#include <stdio.h>
#include "inc/tokens.h"

void printToken(FILE *out, int tk) {
    char *tkname;
    switch (tk) {
        case ID:
            tkname = "ID";
            break;
        case NUM:
            tkname = "NUM";
            break;
        case ASGN:
            tkname = "ASGN";
            break;
        default:
            tkname = "UNKNOWN";
            break;
    }
    fprintf(out, "%s", tkname);
}
