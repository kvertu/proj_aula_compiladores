//<lexer.h>::
#pragma once

#define MAXSTRLEN 256
extern unsigned long linecount;
extern char lexeme[];

extern int getToken(FILE *);
extern unsigned long linecount;
