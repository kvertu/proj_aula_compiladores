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

int symcount = 0;

//procura um nome na tabela de simbolos, se não tiver cadastra um novo símbolo
int lookup(const char *name)
{
    for (int i = 0; i < symcount; i++)
        if (strcmp(symtab[i], name) == 0) return i;

    if (symcount >= SYMTABSIZE) {
        fprintf(stderr, "Tabela de símbolos cheia\n");
        exit(-6);
    }
    strcpy(symtab[symcount], name);
    vmem[symcount] = 0;
    return symcount++;
}

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
            strcpy(name, lexeme);
            match(ID); //ID é associado para variavel r-value ou l-value
            if(lookahead == ASGN){
                // ID :=E -> l-value
                match(ASGN);  
                E();
                // store(name, acc)
                vmem[lookup(name)] = acc;
            }else{
                //r-value
                acc = vmem[lookup(name)];  // recall
            }
            break;
        case OCT:
            acc = strtol(lexeme, NULL, 8);
            match(OCT);
            break;
        case HEX:
            acc = strtol(lexeme, NULL, 16);
            match(HEX);
            break;
        case DEC:
            acc = strtod(lexeme, NULL);
            match(DEC);
            break;    
        default:
            match('('); E(); match(')');
            break;
    }

    if (otimesflag) {
        //fprintf(output, " %c", otimesflag);
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
            acc = acc + pop();
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
            fprintf(stderr, "Atingido fim de arquivo prematuramente (linha %ld)\n", linecount);
        } else if (isprint(lookahead)) {
            fprintf(stderr, "Token '%c' não coincide (esperava '%c') (linha %ld)\n", lookahead, expected, linecount);
        } else {
            fprintf(stderr, "Token %d não coincide (esperava %d) (linha %ld)\n", lookahead, expected, linecount);
        }
        exit(-2);
    }
}
