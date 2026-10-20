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

static void skipToStatementEnd(void) {
    while (lookahead != EOF && lookahead != '\n' && lookahead != ';') {
        lookahead = getToken(source);
    }
    if (lookahead == '\n' || lookahead == ';') {
        match(lookahead);
    }
    if (err) {
        return;
    }
}

void expr(void)
{
    char name[MAXSTRLEN+1];
    int sigflag = 0;
    int otimesflag = 0;
    int oplusflag = 0;
    if (err) {
        return;
    }
    if (lookahead == '+' || lookahead == '-') {
        if (lookahead == '-')
            sigflag = 1; // Sinal identificado
        match(lookahead);
        if (err) {
            return;
        }
    }

    if (lookahead != ID && lookahead != NUM && lookahead != '(') {
        err = 0;
        errtk = 0;
        expectedtk = 0;
        err = PARS_TOKEN_MISMATCH;
        errtk = lookahead;
        errline = linecount;
        expectedtk = '(';
        return;
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
            match('(');
            // Tem que sair de forma precoce para não sobrescrever o erro em matches posteriores
            if (err) return; 
            expr();
            // Mesma coisa aqui
            if (err) return;
            match(')');
            break;
    }

    if (otimesflag) {
        //fprintf(output, " %c", otimesflag);
        if(otimesflag == '*'){
            acc = pop() * acc;
        }else {
            double divisor = acc;
            if (divisor == 0) {
                err = MATH_DIV_BY_ZERO;
                errline = linecount;
                return;
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
        acc = -acc;
        sigflag = 0;
    }

    if (oplusflag) {
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
        lookahead = getToken(source);
    } else {
        err = 0;
        errtk = 0;
        expectedtk = 0;
        errline = linecount;

        if (lookahead == EOF) {
            err = LEX_PREMATURE_EOF;
        } else {
            err = PARS_TOKEN_MISMATCH;
            errtk = lookahead;
            expectedtk = expected;
        }
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

void mybc(void) {
    lookahead = getToken(source);
    while (lookahead != EOF) {
        if (err) {
            printErr();
            skipToStatementEnd();
            continue;
        }

        if (isFIRST()) {
            expr();
            if (err) {
                printErr();
                skipToStatementEnd();
                continue;
            }
            fprintf(output, "%.10g\n", acc);
        } else if (lookahead != '\n' && lookahead != ';') {            
            err = PARS_TOKEN_MISMATCH;        
            errtk = lookahead;
            errline = linecount;
            expectedtk = '(';
            printErr();
            skipToStatementEnd();
            continue;
        } else {
            match(lookahead);
            if (err) {
                printErr();
                skipToStatementEnd();
            }
        }
    }

    if (err) {
        printErr();
    }
}
