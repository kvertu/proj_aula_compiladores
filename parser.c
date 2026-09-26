//<parser.c>::
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/lexer.h"
#include "inc/tokens.h"
#include "inc/parser.h"

int lookahead; // Lookahead - o "olho" do compilador
extern char lexeme[MAXSTRLEN]; // Guarda os tokens

void E(void)
{
    int sigflag = 0;
    int otimesflag = 0;
    int oplusflag = 0;
    if (lookahead == '+' || lookahead == '-') {
        if (lookahead == '-')
            sigflag = 1; // Sinal identificado
        match(lookahead);
    }

_T:
    // T();
_F:
    // F();
    switch (lookahead) {
        case ID:
            fprintf(output, " %s", lexeme);
            match(ID);
            break;
        case DEC:
            match(DEC);
            break;
        case OCT:
            match(OCT);
            break;
        case HEX:
            match(HEX);
            break;
        default:
            match('('); E(); match(')');
            break;
    }

    if (otimesflag) {
        fprintf(output, " %c", otimesflag);
        otimesflag = 0;
    }

    if (lookahead == '*' || lookahead == '/')
    {
        otimesflag = lookahead;
        match(lookahead);
        goto _F;
    }

    // Termo termina aqui
    if (sigflag) {
        fprintf(output, " neg");
        sigflag = 0;
    }

    if (oplusflag) {
        fprintf(output, " %c", oplusflag);
        oplusflag = 0;
    }

    if (lookahead == '+' || lookahead == '-')
    {
        oplusflag = lookahead;
        match(lookahead);
        goto _T;
    }
}

void match(int expected)
{
    if (expected == lookahead) {
        // Esvazia o topo da pilha e segue para o próximo
        //if (isprint(lookahead))
        //    printf("%c ", lookahead);
        //else
        //    printf("%d ", lookahead);

        lookahead = getToken(source);
    } else {
        // Erro de sintaxe
        if (lookahead == EOF) {
            fprintf(stderr, "Atingido fim de arquivo prematuramente (linha %ld)\n", linecount);
        } else if (isprint(lookahead)) {
            fprintf(stderr, "Token '%c' não coincide (esperava '%c') (linha %ld)\n", lookahead, expected, linecount);
        } else {
            fprintf(stderr, "Token %d não coincide (esperava %d) (linha %ld)\n", lookahead, expected, linecount);
        }
        exit(-2);
    }
}
