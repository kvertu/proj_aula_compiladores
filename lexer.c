#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "inc/error.h"
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
    while ((c = getc(tape)) != EOF && isspace(c)) {
        if (c == '\n') {
            linecount++;
            break;
        }
    }
    if (c != EOF) {
        ungetc(c, tape);
    }
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
	head = getc(tape);
	if (head == EOF) {
		return;
	}
	if (head == '{') {
		while ((head = getc(tape)) != EOF) {
			if (head == '\\') {
				int next = getc(tape);
				if (next == '{' || next == '}') {
					head = getc(tape);
				} else if (next != EOF) {
					ungetc(next, tape);
				}
			}

			if (head == '\n') {
				linecount++;
			}

			if (head == '}') {
				break;
			}
		}
		if (head == EOF) {
			err = LEX_PREMATURE_EOF;
			errline = linecount;
			return;
		}
		goto _skpspc;
	}
	ungetc(head, tape);
}

int isID(FILE *tape)
{
    int c = getc(tape);

    if (c == EOF) {
        lexeme[0] = 0;
        return 0;
    }

    if (isalpha((unsigned char)c))
    {
        lexeme[0] = (char)c;
        int i = 1;
        while ((c = getc(tape)) != EOF && isalnum((unsigned char)c))
        {
            if (i < MAXSTRLEN) {
                lexeme[i] = (char)c;
                i++;
            }
        }
        if (c != EOF) {
            ungetc(c, tape);
        }
        if (i < MAXSTRLEN) {
            lexeme[i] = 0;
        } else {
            lexeme[MAXSTRLEN] = 0;
        }
        return ID;
    }

    ungetc(c, tape);
    lexeme[0] = 0;
    return 0;
}

// ASGN = :=
int isASGN(FILE *tape){
    // ":="
    int c = getc(tape);
    if (c == EOF) {
        lexeme[0] = 0;
        return 0;
    }
    if (c == ':') {
        int next = getc(tape);
        if (next == '=') {
            lexeme[0] = ':';
            lexeme[1] = '=';
            lexeme[2] = 0;
            return ASGN;
        }
        if (next != EOF) {
            ungetc(next, tape);
        }
    }
    ungetc(c, tape);
    lexeme[0] = 0;
    return 0;
}

// NUM = [0-9]+ EE | [0-9]+\.[0-9]* | [0-9]*\.[0-9]+
int isNUM(FILE *tape) {
    int c = getc(tape);
    if (c != EOF && isdigit((unsigned char)c)) {
        double aux;
        ungetc(c, tape);
        int nc = fscanf(tape, "%lg", &aux);
        if (nc == EOF) {
            lexeme[0] = 0;
            return 0;
        }
        sprintf(lexeme, "%.17g", aux);
        return NUM;
    }
    if (c != EOF) {
        ungetc(c, tape);
    }
    lexeme[0] = 0;
    return 0;
}

int getToken(FILE *src)
{
    skipcomments(src);
    if (err == LEX_PREMATURE_EOF) {
        return EOF;
    }

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
