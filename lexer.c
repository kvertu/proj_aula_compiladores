#include <stdio.h>
#include <ctype.h>
#include "inc/tokens.h"
#include "inc/lexer.h"

unsigned long linecount = 1;

void skipWhitespaces(FILE *tape)
{
    /*
     * Pula os espaços em branco
     */
    int c;
    while (isspace(c = getc(tape)))
    {
        if (c == '\n') linecount++;
    }
    ungetc(c, tape);
}

int isID(FILE *tape)
{
    /*
     * ID = [a-Z][a-Z0-9]*
     */
    int head = getc(tape);

    if (isalpha(head))
    {
        while (isalnum(head = getc(tape)));
        ungetc(head, tape);
        return ID;
    }

    ungetc(head, tape);
    return 0;
}

int isOCT(FILE *tape)
{
    /*
     * OCT = '0'[0-7]+
     */
    int head = getc(tape);

    if (head == '0') {
        int octcheck = getc(tape);
        if ('0' > head || head > '7')
        {
            // Não é um octal, coloca tudo de volta na tape
            ungetc(octcheck, tape);
            ungetc(head, tape);
            return 0;
        }
        while (head = getc(tape), '0' <= head && head <= '7');
        ungetc(head, tape);
        return OCT;
    }

    ungetc(head, tape);
    return 0;
}

int isHEX(FILE *tape)
{
    /*
     * HEX = '0'[xX][0-9A-Fa-f]+
     */
    char hprefix[2]; // Prefixo "0x"
    hprefix[0] = getc(tape);
    hprefix[1] = getc(tape);

    if (hprefix[0] == '0' && tolower(hprefix[1]) == 'x') {
        int head;
        if (isxdigit(head = getc(tape)))
        {
            while (isxdigit(head = getc(tape)));
            ungetc(head, tape);

            return HEX;
        }
        // Não é um hexadecimal, devolvendo tudo
        ungetc(head, tape);
    }

    ungetc(hprefix[1], tape);
    ungetc(hprefix[0], tape);
    return 0;
}

int isDEC(FILE *tape)
{
    /*
     * DEC = '0' | [1-9][0-9]*
     */
    int head = getc(tape);

    if (isdigit(head)) {
        if (head == '0')
            return DEC;

        while (isdigit(head = getc(tape)));
        ungetc(head, tape);
        return DEC;
    }

    ungetc(head, tape);
    return 0;
}

int getToken(FILE *src)
{
    skipWhitespaces(src);

    int token;
    fprintf(stderr, "Verificando ID...\n");
    if ((token = isID(src))) return token;
    fprintf(stderr, "Verificando OCT...\n");
    if ((token = isOCT(src))) return token;
    fprintf(stderr, "Verificando HEX...\n");
    if ((token = isHEX(src))) return token;
    fprintf(stderr, "Verificando DEC...\n");
    if ((token = isDEC(src))) return token;
    fprintf(stderr, "Nada encontrado, retornando token genérico...\n");

    // Se nenhum padrão foi satisfeito, retorne o caracter ASCII mesmo
    token = getc(src);

    return token;
}
