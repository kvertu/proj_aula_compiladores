#include <stdio.h>
#include "inc/main.h"
#include "inc/parser.h"

FILE *source;
FILE *output;

int main()
{
    source = stdin;
    output = stdout;
    // source = fopen(argv[1], "r");
    lookahead = getToken(source);

    E();

    // printf("\n");

    return 0;
}
