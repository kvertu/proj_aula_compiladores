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

// Gramática EBNF do MyBC anotada com ações semânticas (vide Aula 07).
// Na gramática, as ações aparecem como [[n]]; no código, cada ação n fica
// delimitada por /*n*/ ... /*n*/, separando a sintaxe da interpretação.
//
//   mybc -> { [ E [[11]] ] ( '\n' | ';' ) } EOF
//   E    -> [ [[1]] ( '+' | '-' ) ] T [[8]] { [[9]] ( '+' | '-' ) T [[10]] }
//   T    -> F { [[6]] ( '*' | '/' ) F [[7]] }
//   F    -> [[2]] ID ( ASGN E [[3]] | [[4]] ) | [[5]] NUM | '(' E ')'
//
// Ações semânticas:
//   [[1]]  sigflag <- (lookahead == '-')      sinal unário da expressão
//   [[2]]  name <- lexeme                     guarda o nome antes do match(ID)
//   [[3]]  store(name)                        atribuição: name := acc (l-value)
//   [[4]]  acc <- recall(name)                leitura da variável (r-value)
//   [[5]]  acc <- strtod(lexeme)              valor da constante numérica
//   [[6]]  push(); otimesflag <- lookahead    empilha o operando esquerdo de '*' '/'
//   [[7]]  acc <- pop() otimes acc            multiplica/divide (checa divisão por zero)
//   [[8]]  if sigflag: acc <- -acc            aplica o sinal unário ao primeiro termo
//   [[9]]  push(); oplusflag <- lookahead     empilha o operando esquerdo de '+' '-'
//   [[10]] acc <- pop() oplus acc             soma/subtrai
//   [[11]] print acc                          exibe o resultado da expressão

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
        /*1*/ if (lookahead == '-') sigflag = 1; /*1*/
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
            /*2*/ strcpy(name, lexeme); /*2*/
            match(ID); //ID é associado para variavel r-value ou l-value
            if(lookahead == ASGN){
                // ID :=E -> l-value
                match(ASGN);
                expr();
                /*3*/ store(name); /*3*/
            }else{
                //r-value
                /*4*/ acc = recall(name); /*4*/
            }
            break;
        case NUM:
            /*5*/ acc = strtod(lexeme, NULL); /*5*/
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

    /*7*/
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
    /*7*/

    if (lookahead == '*' || lookahead == '/')
    {
        /*6*/ push(); otimesflag = lookahead; /*6*/ //guarda operando esquerdo
        match(lookahead);
        goto _F;
    }

    // Termo termina aqui
    /*8*/
    if (sigflag) {
        acc = -acc;
        sigflag = 0;
    }
    /*8*/

    /*10*/
    if (oplusflag) {
        if(oplusflag == '+'){
            acc = pop() + acc;
        }else{
            acc = pop() - acc;
        }
        oplusflag = 0;
    }
    /*10*/

    if (lookahead == '+' || lookahead == '-')
    {
        /*9*/ push(); oplusflag = lookahead; /*9*/
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
            /*11*/ fprintf(output, "%lg\n", acc); /*11*/
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
