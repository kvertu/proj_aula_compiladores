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
        if (c == '\n') {
            linecount++;
            break;
        }
    }
    ungetc(c, tape);
}

void skipcomments(FILE *tape)
{
	int head;
	_skpspc:
	skipspaces(tape);
	if ( (head = getc(tape)) == '{') {
		while( (head = getc(tape)) != '}') {

			if (head == '\\') {
				if ( (head = getc(tape)) == '{' || head == '}')
					continue;
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

// ASGN = :=
int isASGN(FILE *tape){
    // ":="
    lexeme[0] = getc(tape);
    if (lexeme[0] == ':') {
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') {
            return ASGN;
        }
        ungetc(lexeme[1], tape);
    }
    ungetc(lexeme[0], tape);
    return 0;
}

// NUM = [0-9]+ EE | [0-9]+\.[0-9]* | [0-9]*\.[0-9]+
int isNUM(FILE *tape) {
    if (isdigit(lexeme[0] = getc(tape))) {
        double aux;
        ungetc(lexeme[0], tape);
        int nc = fscanf(tape, "%lg", &aux);
        if (nc == EOF) {
            lexeme[0] = 0;
            return 0;
        }
        sprintf(lexeme, "%.17g", aux);
        return NUM;
    }
    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

int getToken(FILE *src)
{
    skipcomments(src);

    int token;
    //fprintf(stderr, "Verificando ID...\n");
    if ((token = isID(src))) return token;
    //fprintf(stderr, "Verificando ASGN...\n");
    if((token = isASGN(src))) return token;
    // //fprintf(stderr, "Verificando NUM...\n");
    if ((token = isNUM(src))) return token;
    //fprintf(stderr, "Nada encontrado, retornando token genérico...\n");

    // Se nenhum padrão foi satisfeito, retorne o caracter ASCII mesmo
    token = getc(src);

    return token;
}
