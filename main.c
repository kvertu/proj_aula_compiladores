#include <stdio.h>
#include "inc/main.h"
#include "inc/parser.h"

FILE *source;
FILE *output;

//falta o "verificador" de sinal 

int main()
{
    source = stdin;
    output = stdout;
    // source = fopen(argv[1], "r");
    lookahead = getToken(source);

    E();

    printf("\n");

    return 0;
}
