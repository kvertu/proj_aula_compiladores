// <symtab.c>::
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/error.h"
#include "inc/lexer.h"
#include "inc/symtab.h"

//memoria virtual, tem o msm tamanho tabela de simbolos
double vmem[SYMTABSIZE];
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
        return -1;
    }
    return inserted;
}

// Implementação da Thais (une a busca e o append em um só, pode não ser uma boa)
//procura um nome na tabela de simbolos, se não tiver cadastra um novo símbolo
int lookup(const char *name)
{
    for (int i = 0; i < symcount; i++)
        if (strcmp(symtab[i], name) == 0) return i;

    if (symcount >= SYMTABSIZE) {
        fprintf(stderr, "Tabela de símbolos cheia\n");
        exit(-6); // TODO: remover essas chamadas de exit, substituir por um tratamento de erro mais robusto
    }
    strcpy(symtab[symcount], name);
    vmem[symcount] = 0;
    return symcount++;
}
