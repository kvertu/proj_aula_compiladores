#pragma once

// Será definido no main
extern FILE *source;
extern FILE *output;

extern int lookahead;

extern void E(void);
extern void T(void);
extern void R(void);
extern void F(void);
extern void Q(void);
extern void match(int);

extern int getToken(FILE *); // Definido em lexer.c
extern unsigned long linecount; // Definido em lexer.c
