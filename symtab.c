// <symtab.c>::
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/error.h"
#include "inc/lexer.h"
#include "inc/symtab.h"

char symtab[SYMTABSIZE][MAXSTRLEN+1];  // Nomes das variáveis declaradas
int symcount = 0;                       // Quantidade de entradas ocupadas

/*
 * symtab_lookup — procura uma variável na tabela de símbolos.
 *
 * Retorna o índice da variável (que também é seu endereço em vmem),
 * ou -1 se ela não estiver na tabela.
 */
int symtab_lookup(const char* varname) {
    int i;
    for (i = 0; i < SYMTABSIZE; i++) {
        if (strcmp(varname, symtab[i]) == 0) {
            return i;
        }
    }
    return -1; // not found
}

/*
 * symtab_append — insere uma nova variável ao final da tabela.
 *
 * Retorna o índice atribuído. Se a tabela estiver cheia, sinaliza
 * SYM_OVERFLOW em err e retorna esse código (negativo).
 */
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

