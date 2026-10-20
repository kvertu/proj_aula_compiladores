// <symtab.c>::
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/error.h"
#include "inc/lexer.h"
#include "inc/symtab.h"

char symtab[SYMTABSIZE][MAXSTRLEN+1];
int symcount = 0;

int symtab_lookup(const char* varname) {
    int i;
    for (i = 0; i < SYMTABSIZE; i++) {
        if (strcmp(varname, symtab[i]) == 0) {
            return i;
        }
    }
    return -1; // not found
}

int symtab_append(const char* varname) {
    int inserted;
    if (symcount < SYMTABSIZE) {
        strcpy(symtab[inserted = symcount], varname);
        symcount++;
    } else {
        // symtab overflow
        err = SYM_OVERFLOW;
        return SYM_OVERFLOW;
    }
    return inserted;
}

