#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "inc/tokens.h"
#include "inc/lexer.h"

unsigned long linecount = 1;

char lexeme[MAXSTRLEN+1];
void skipspaces(FILE *tape)
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

/*
	2026-09-09: Implementem um varredor de comentários Pascal [void skipcomments(FILE *)], onde
	um comentário começa com '{' e finaliza com '}', a menos que se utilize
	"\{" e "\}", para ser interpretado como elemento do texto.
	Exemplo:
	begin {
			isto é um comentário. Aqui, eu devo utilizar \{ e \} para constar no interior
		    do comentário
		  }     { outro comentário depois do comentário prévio }
	  blah blah blah
	end;
    TODO: Identificar o erro descrito pelo professor em aula.
 */

void skipcomments(FILE *tape)
{
	int head;
	_skpspc:
	skipspaces(tape);
	if ( (head = getc(tape)) == '{') {
		while( (head = getc(tape)) != '}') {

			if (head == '\\') {
				if ( (head = getc(tape)) == '{' || head == '}')
					head = getc(tape);
			}

			if (head == '\n') {
				linecount++;
			}

			if ( head == EOF ) {
				fprintf(stderr, "Comentário não fechado (linha %ld)\n", linecount);
				exit(-4);
			}
		}
		goto _skpspc;
	}
	ungetc(head, tape);
}

int isID(FILE *tape)
{
    lexeme[0] = getc(tape);

    if (isalpha(lexeme[0]))
    {
        int i = 1;
        while (isalnum(lexeme[i] = getc(tape)))
        {
            if (i < MAXSTRLEN) i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return ID;
    }

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

int isOCT(FILE *tape)
{
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

// ASGN = :=
int isASGN(FILE *tape){
    int head = getc(tape);

    if (head == '=')
        return ASGN;

    if (head == ':') {
        int next = getc(tape);
        if (next == '=')
            return ASGN;
        ungetc(next, tape);
    }

    ungetc(head, tape);
    return 0;

}



int isHEX(FILE *tape)
{
    int hprefix[2]; // Prefixo "0x"
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

int isDEC(FILE *tape)// tinha q alterar
{

    lexeme[0] = getc(tape);

    if (isdigit(lexeme[0])) {
        if (head == '0')
            return DEC;

        int i;    
        while (isdigit(lexeme[i] = getc(tape)));
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return DEC;
    }

    ungetc(lexeme[0], tape);
    return 0;
}

int getToken(FILE *src)
{
    skipcomments(src);

    int token;
    //fprintf(stderr, "Verificando ID...\n");
    if ((token = isID(src))) return token;
    //fprintf(stderr, "Verificando OCT...\n");
    if ((token = isOCT(src))) return token;
    //fprintf(stderr, "Verificando ASGN...\n");
    if((token = isASGN(scr))) return token;
    //fprintf(stderr, "Verificando HEX...\n");
    if ((token = isHEX(src))) return token;
    //fprintf(stderr, "Verificando DEC...\n");
    if ((token = isDEC(src))) return token;
    //fprintf(stderr, "Nada encontrado, retornando token genérico...\n");

    // Se nenhum padrão foi satisfeito, retorne o caracter ASCII mesmo
    token = getc(src);

    return token;
}
