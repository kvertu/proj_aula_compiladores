#pragma once

// Será definido no main
extern FILE *source;
extern FILE *output;

extern int lookahead;
extern char lexeme[]; // Guarda os tokens

extern void mybc();
extern void match(int);
