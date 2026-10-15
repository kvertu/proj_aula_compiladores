//<parser.c>::
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/error.h"
#include "inc/lexer.h"
#include "inc/tokens.h"
#include "inc/vmachine.h"
#include "inc/parser.h"

//usar fscanf em vez de getc

int lookahead; // Lookahead - o "olho" do compilador

void expr(void)
{
    char name[MAXSTRLEN+1];
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
            //fprintf(output, " %s", lexeme);
            strcpy(name, lexeme);
            match(ID); //ID é associado para variavel r-value ou l-value
            if(lookahead == ASGN){
                // ID :=E -> l-value
                match(ASGN);
                expr();
                // store(name)
                store(name);
            }else{
                //r-value
                acc = recall(name);
            }
            break;
        case NUM:
            acc = strtod(lexeme, NULL);
            match(NUM);
            break;
        default:
            match('('); expr(); match(')');
            break;
    }

    if (otimesflag) {
        //fprintf(output, " %c", otimesflag);
        if(otimesflag == '*'){
            acc = pop() * acc;
        }else {
            double divisor = acc;
            if (divisor == 0) {
                // fprintf(stderr, "Divisão por zero (linha %ld)\n", linecount);
                err = MATH_DIV_BY_ZERO;
                errline = linecount;
                // exit(-5);
            } else {
                acc = pop() / divisor;
            }
        }
        otimesflag = 0;
    }

    if (lookahead == '*' || lookahead == '/')
    {
        push(); //guarda operando esquerdo
        otimesflag = lookahead;
        match(lookahead);
        goto _F;
    }

    // Termo termina aqui
    if (sigflag) {
        //fprintf(output, " neg");
        acc = -acc;
        sigflag = 0;
    }

    if (oplusflag) {
        //fprintf(output, " %c", oplusflag);
        if(oplusflag == '+'){
            acc = pop() + acc;
        }else{
            acc = pop() - acc;
        }
        oplusflag = 0;
    }

    if (lookahead == '+' || lookahead == '-')
    {
        push();
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
            // fprintf(stderr, "Atingido fim de arquivo prematuramente (linha %ld)\n", linecount);
            err = LEX_PREMATURE_EOF;
        } else {
            err = PARS_TOKEN_MISMATCH;
            errtk = lookahead;
            errline = linecount;
            expectedtk = expected;
        }
        // exit(-2);
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
    lookahead = getToken(source);
    while(lookahead != EOF) {
        if (isFIRST()) {
            expr(); // Resultado fica na acc
            if (err) {
                printErr();
            } else {
                fprintf(output, "%lg\n", acc);
            }
        } // Podemos ter a expressão vazia

        if (lookahead == '\n' || lookahead == ';') {
            match(lookahead);
        }
    }
}
