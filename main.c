#include <stdio.h>
#include "inc/main.h"
#include "inc/parser.h"

FILE *source;
FILE *output;
FILE *outerror;

int main()
{
    source = stdin;
    output = stdout;
    outerror = stderr;

    mybc();

    return 0;
}
