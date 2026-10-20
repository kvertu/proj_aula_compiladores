// <vmachine>::
#include <stdio.h>
#include <stdlib.h>
#include "inc/symtab.h"
#include "inc/error.h"
#include "inc/vmachine.h"

//maquina virtual definção
//acumulador:
double acc = 0;

//memoria virtual, tem o msm tamanho tabela de simbolos
double vmem[SYMTABSIZE];
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
        if ((pos = symtab_append(name)) < 0) {
            printErr();
            return;
        }
    }
    vmem[pos] = acc;
}

// busca o valor de name na vmem e o devolve
double recall(const char *name)
{
    int pos = symtab_lookup(name);
    if (pos < 0) {
        if ((pos = symtab_append(name)) < 0) {
            printErr();
            return 0;
        }
        vmem[pos] = 0;
    }
    return vmem[pos];
}
