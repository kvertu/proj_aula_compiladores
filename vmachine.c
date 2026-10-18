// <vmachine>::
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

void store(const char *name)
{
    int pos = lookup(name);
    if (pos < 0) {
        return;
    }
    vmem[pos] = acc;
}

double recall(const char *name)
{
    int pos = lookup(name);

    if (pos < 0) {
        return 0.0;
    }

    return vmem[pos];  // recall
}
