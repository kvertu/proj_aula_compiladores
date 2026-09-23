//<parser.c>::
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/lexer.h"
#include "inc/tokens.h"
#include "inc/parser.h"

// Lookahead - o "olho" do compilador
int lookahead;

/*
 * LL(1) parser top-down (gramática preditiva)
 * Requer uma gramática LL(1)
 *
 * LL(1) para expressões
 * E -> ['+''-'] T R
 * T -> F Q
 * R -> ['+''-'] T R | <vazio>
 * Q -> ['*''/'] F Q | <vazio>
 * F -> ID | DEC | '(' E ')'
 */

void E(void)
{
    // E -> ['+''-'] T R
    // Verificar se tem prefixo
    if (lookahead == '+' || lookahead == '-')
        match(lookahead);

    T();

    // Equivale ao R
    while (lookahead == '+' || lookahead == '-')
    {
        match(lookahead); T();
    }
}

void T(void)
{
    // T -> F Q
    F();

    // Equivale ao Q
    while (lookahead == '*' || lookahead == '/')
    {
        match(lookahead); F();
    }
}

// void R(void)
// {
//     // R -> ['+''-'] T R | <vazio>
//     if (lookahead == '+' || lookahead == '-')
//     {
//         match(lookahead); T(); R(); // Mau uso da pilha
//     }
// }

// void Q(void)
// {
//     // Q -> ['*''/'] F Q | <vazio>
//     if (lookahead == '*' || lookahead == '/')
//     {
//         match(lookahead); F(); Q(); // Mau uso da pilha
//     }
// }

void F(void)
{
    // F -> ID | DEC | OCT | HEX | '(' E ')'
    switch (lookahead) {
        case ID:
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
}

void match(int expected)
{
    if (expected == lookahead) {
        // Esvazia o topo da pilha e segue para o próximo
        if (isprint(lookahead))
            printf("%c ", lookahead);
        else
            printf("%d ", lookahead);

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
