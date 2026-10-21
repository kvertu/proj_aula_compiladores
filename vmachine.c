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

/*
 * push — empilha o valor atual do acumulador.
 */
void push(void)
{
    sp++;
    stack[sp] = acc;
}

/*
 * pop — desempilha e retorna o valor do topo da pilha.
 */
double pop(void)
{
    double aux = stack[sp];
    sp--;
    return aux;
}

/*
 * store — atribuição: grava o acumulador na variável name.
 *
 * Se a variável ainda não existir, ela é criada na tabela de símbolos.
 */
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

/*
 * recall — leitura: retorna o valor da variável name.
 *
 * Variáveis ainda não atribuídas são criadas com valor 0, como no bc.
 */
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
