// <vmachine>::
#include <stdio.h>
#include <stdlib.h>
#include "inc/symtab.h"
#include "inc/vmachine.h"

//maquina virtual definção
//acumulador:
double acc = 0;

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

//guarda o valor atual do acumulador na váriavel que já esta na tabela de simbolos
void store(const char *name)
{
    int pos = symtab_lookup(name);
    if (pos < 0) {
        pos = symtab_append(name);
        if (pos < 0) {
            fprintf(stderr, "Tabela de símbolos cheia\n");
        exit(-6);
        }
        vmem[pos] = 0;
    }
    vmem[pos] = acc;
}

// busca o valor de name na vmem e o devolve
double recall(const char *name)
{
    int pos = symtab_lookup(name);
    if (pos < 0) {
        pos = symtab_append(name);
        if (pos < 0) {
            fprintf(stderr, "Tabela de símbolos cheia\n");
            exit(-6);
        }
        vmem[pos] = 0;
    }
    return vmem[pos];
}
