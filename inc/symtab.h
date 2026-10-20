// <stmtab.h>::
#pragma once
#include "lexer.h"

#define SYMTABSIZE 1024
extern double vmem[];
extern char symtab[SYMTABSIZE][MAXSTRLEN+1];
extern int symcount;
extern int symtab_lookup(const char* varname);
extern int symtab_append(const char* varname);
