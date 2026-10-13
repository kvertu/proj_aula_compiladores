//<parser.c>::
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/lexer.h"
#include "inc/tokens.h"
#include "inc/parser.h"

//usar fscanf em vez de getc
//tem q mudar pra ponto flutuante, criar ponto flutuante

int lookahead; // Lookahead - o "olho" do compilador
extern char lexeme[]; // Guarda os tokens

//maquina virtual definção
//acumulador: 
double acc = 0;
#define MAXSTACKSIZE 1024
double stack[MAXSTACKSIZE];
int sp = -1; //stack pointer
void push(void)
{
    sp++;
    stack[sp] = acc;
}
double pop(void)
{
    double aux = stack[sp];
    sp--;
    return aux;
}


//memoria virtual, tem o msm tamanho tabela de simbolos
#define SYMTABSIZE 1024
double vmem[SYMTABSIZE];
char symtab[SYMTABSIZE][MAXSTRLEN+1];

void E(void)
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
            stpcpy(name, lexeme);
            //depois do identificador pode vir dois pontos ou igual (tarefa isASGN)
            match(ID); //ID é associado para variavel r-value ou l-value
            if(lookahead == ASGN){
                // ID :=E -> l-value
                // store(name, acc)
                match(ASGN);
                E();
            }else{
                //r-value
                //acc = recall(name); //adicionar vmem em algum lugar
                ;

            }

            break;
        case DEC:
            acc = atoi(lexeme);
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
        //fprintf(output, " %c", otimesflag);
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
        //fprintf(output, " neg");
        sigflag = 0;
    }

    if (oplusflag) {
        //fprintf(output, " %c", oplusflag);
        if(oplusflag == '+'){
            acc = acc + pop();
        }else{
            // subtract here ---- :to do
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
            fprintf(stderr, "Atingido fim de arquivo prematuramente (linha %ld)\n", linecount);
        } else if (isprint(lookahead)) {
            fprintf(stderr, "Token '%c' não coincide (esperava '%c') (linha %ld)\n", lookahead, expected, linecount);
        } else {
            fprintf(stderr, "Token %d não coincide (esperava %d) (linha %ld)\n", lookahead, expected, linecount);
        }
        exit(-2);
    }
}
