// <error.c>::
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/tokens.h"
#include "inc/error.h"

// Estado do erro corrente (zerado após cada printErr)
int err = 0;            // Código do erro (0 = nenhum)
long errline = 0;       // Linha onde o erro ocorreu
int errtk = 0;          // Token encontrado
int expectedtk = 0;     // Token esperado

/*
 * printTokenLike — escreve em out uma forma legível de um token.
 *
 * Tokens especiais aparecem pelo nome (EOF, LF, ID, NUM, ASGN);
 * caracteres imprimíveis, entre aspas simples; os demais, pelo código.
 */
static void printTokenLike(FILE *out, int tk) {
    switch (tk) {
        case EOF:
            fprintf(out, "EOF");
            return;
        case '\n':
            fprintf(out, "LF");
            return;
        case ID:
            fprintf(out, "ID");
            return;
        case NUM:
            fprintf(out, "NUM");
            return;
        case ASGN:
            fprintf(out, "ASGN");
            return;
    }
    if (isprint(tk)) {
        fprintf(out, "'%c'", tk);
    } else {
        fprintf(out, "UNKNOWN (%d)", tk);
    }
}

/*
 * printErr — reporta o erro corrente em outerror e zera o estado de erro.
 *
 * SYM_OVERFLOW é fatal: o programa é encerrado com esse código.
 */
void printErr() {
    fprintf(outerror, "ERRO: ");
    switch (err) {
        case SYM_OVERFLOW:
            // Não tem como se recuperar de um erro desses, melhor encerrar mesmo
            fprintf(outerror, "Atingido limite da tabela de símbolos. (FATAL)\n");
            exit(SYM_OVERFLOW);
            break;
        case LEX_PREMATURE_EOF:
            fprintf(outerror, "Atingido fim de arquivo prematuramente.\n");
            break;
        case MATH_DIV_BY_ZERO:
            fprintf(outerror, "Divisão por zero na linha %ld\n", errline);
            break;
        case PARS_TOKEN_MISMATCH:
            fprintf(outerror, "Token inesperado ");
            printTokenLike(outerror, errtk);
            fprintf(outerror, " encontrado na linha %ld (esperava ", errline);
            printTokenLike(outerror, expectedtk);
            fprintf(outerror, ")\n");
            break;
        default:
            fprintf(outerror, "Problema desconhecido.\n");
    }

    err = 0;
    errtk = 0;
    expectedtk = 0;
    errline = 0;
}
