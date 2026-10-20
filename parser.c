//<parser.c>::
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/lexer.h"
#include "inc/tokens.h"
#include "inc/vmachine.h"
#include "inc/parser.h"

//usar fscanf em vez de getc
//tem q mudar pra ponto flutuante, criar ponto flutuante

/*
 * Gramática EBNF do MyBC com ações semânticas (vide Aula 07)
 * ----------------------------------------------------------
 *
 * mybc -> { [ E [11] ] ( '\n' | ';' ) }
 *
 * E    -> [ [1] ('+' | '-') ] T [2] { [3] ('+' | '-') T [4] }
 * T    -> F { [5] ('*' | '/') F [6] }
 * F    -> [7] ID ( ASGN E [8] | [9] )
 *       | [10] NUM
 *       | '(' E ')'
 *
 * Convenção das ações semânticas:
 *
 * [1]  ⊖.t ← lookahead       : memoriza o sinal unário (sigflag)
 * [2]  if ⊖.t = '-': acc ← -acc : aplica a negação ao primeiro termo
 * [3]  push acc; ⊕.t ← lookahead : guarda operando esquerdo e o operador aditivo
 * [4]  acc ← pop ⊕.t acc     : executa a soma/subtração pendente
 * [5]  push acc; ⊗.t ← lookahead : guarda operando esquerdo e o operador multiplicativo
 * [6]  acc ← pop ⊗.t acc     : executa a multiplicação/divisão pendente
 * [7]  name ← lexeme         : memoriza o identificador antes do match(ID)
 * [8]  store(name)           : atribuição (ID como l-value)
 * [9]  acc ← recall(name)    : leitura de variável (ID como r-value)
 * [10] acc ← NUM.t           : carrega a constante numérica no acumulador
 * [11] print acc             : exibe o resultado da expressão
 *
 * No código, cada ação semântica [N] fica entre um par de comentários
 * contendo seu número N (abrindo e fechando a ação), separando-a do que é
 * puramente sintático (match, laços, desvios). Exemplo, para a ação [9]:
 *     acc = recall(name);   envolvida pelos marcadores de número 9
 */

int lookahead; // Lookahead - o "olho" do compilador

void expr(void)
{
    char name[MAXSTRLEN+1];
    int sigflag = 0;
    int otimesflag = 0;
    int oplusflag = 0;
    if (lookahead == '+' || lookahead == '-') {
        /*1*/ if (lookahead == '-') sigflag = 1; /*1*/
        match(lookahead);
    }

_T:
    // T();
_F:
    // F();
    switch (lookahead) {
        case ID:
            /*7*/ strcpy(name, lexeme); /*7*/
            match(ID); //ID é associado para variavel r-value ou l-value
            if(lookahead == ASGN){
                // ID :=E -> l-value
                match(ASGN);
                expr();
                /*8*/ store(name); /*8*/
            }else{
                //r-value
                /*9*/ acc = recall(name); /*9*/
            }
            break;
        // case OCT:
        //     acc = strtol(lexeme, NULL, 8);
        //     match(OCT);
        //     break;
        // case HEX:
        //     acc = strtol(lexeme, NULL, 16);
        //     match(HEX);
        //     break;
        case NUM:
            /*10*/ acc = strtod(lexeme, NULL); /*10*/
            match(NUM);
            break;
        default:
            match('('); expr(); match(')');
            break;
    }

    /*6*/
    if (otimesflag) {
        if(otimesflag == '*'){
            acc = pop() * acc;
        }else {
            double divisor = acc;
            if (divisor == 0) {
                fprintf(stderr, "Divisão por zero (linha %ld)\n", linecount);
                exit(-5);
            }
            acc = pop() / divisor;
        }
        otimesflag = 0;
    }
    /*6*/

    if (lookahead == '*' || lookahead == '/')
    {
        /*5*/ push(); otimesflag = lookahead; /*5*/
        match(lookahead);
        goto _F;
    }

    // Termo termina aqui
    /*2*/
    if (sigflag) {
        acc = -acc;
        sigflag = 0;
    }
    /*2*/

    /*4*/
    if (oplusflag) {
        if(oplusflag == '+'){
            acc = pop() + acc;
        }else{
            acc = pop() - acc;
        }
        oplusflag = 0;
    }
    /*4*/

    if (lookahead == '+' || lookahead == '-')
    {
        /*3*/ push(); oplusflag = lookahead; /*3*/
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

int isFIRST(void) {
    switch (lookahead) {
        case '+':
        case '-':
        case ID:
        case NUM:
        case '(':
            return lookahead;
        default:
            return 0;
    }
}

// TODO: criar uma flag de erro para tratar corretamente aqui
void mybc(void) {
    while(lookahead != EOF) {
        if (isFIRST()) {
            expr(); // Resultado fica na acc
            /*11*/ fprintf(output, "%lg\n", acc); /*11*/
        } // Podemos ter a expressão vazia

        if (lookahead == '\n' || lookahead == ';') {
            match(lookahead);
        }
    }
}
