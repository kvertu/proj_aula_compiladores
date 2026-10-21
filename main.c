// <main.c>::
#include <stdio.h>
#include "inc/parser.h"
#include "inc/main.h"

FILE *source;       // Fita de entrada
FILE *output;       // Saída dos resultados
FILE *outerror;     // Saída das mensagens de erro

/*
 * main — ponto de entrada: liga os fluxos padrão e inicia o interpretador.
 */
int main()
{
    source = stdin;
    output = stdout;
    outerror = stderr;

    mybc();

    return 0;
}
