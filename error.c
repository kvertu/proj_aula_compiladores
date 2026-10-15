// <error.c>::
#include <ctype.h>
#include <stdio.h>
#include "inc/tokens.h"
#include "inc/error.h"

int err = 0;
long errline;
int errtk;
int expectedtk;

void printErr() {
    fprintf(outerror, "ERRO: ");
    switch (err) {
        case SYM_OVERFLOW:
            fprintf(outerror, "Atingido limite da tabela de símbolos.\n");
            break;
        case LEX_PREMATURE_EOF:
            fprintf(outerror, "Atingido fim de arqivo prematuramente.\n");
            break;
        case MATH_DIV_BY_ZERO:
            fprintf(outerror, "Divisão por zero na linha %ld\n", errline);
            break;
        case PARS_TOKEN_MISMATCH:
            fprintf(outerror, "Token inesperado '");
            if (isprint(errtk)) {
                fprintf(outerror, "%c", errtk);
            } else {
                printToken(outerror, errtk);
            }
            fprintf(outerror, "' encontrado na linha %ld (esperava '", errline);
            if (isprint(expectedtk)) {
                fprintf(outerror, "%c", expectedtk);
            } else {
                printToken(outerror, expectedtk);
            }
            fprintf(outerror, "')\n");
            break;
        default:
            fprintf(outerror, "Problema desconhecido.\n");
    }

    err = 0; // Limpa a mensagem de erro
}
