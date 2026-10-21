// <lexer.c>::
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "inc/tokens.h"
#include "inc/error.h"
#include "inc/lexer.h"


unsigned long linecount = 1;        // Linha corrente da fita (usada nas mensagens de erro)

char lexeme[MAXSTRLEN+1];           // Texto do último token reconhecido

/*
 * skipspaces — descarta os espaços em branco da fita.
 *
 * Para no primeiro caractere não-branco ou em um '\n', devolvendo ele à
 * fita: a quebra de linha é um token da gramática (separador de comandos)
 * e por isso não pode ser descartada. Ao encontrar '\n', incrementa
 * linecount.
 */
void skipspaces(FILE *tape)
{
    int c;
    // Consome espaços em branco até um caractere ou um '\n'
    while (isspace(c = getc(tape)))
    {
        if (c == '\n') {
            linecount++;
            break;
        }
    }
    ungetc(c, tape);
}

/*
 * skipcomments — descarta espaços e comentários até o início do próximo token.
 *
 * Comentários são delimitados por chaves: { ... }. Dentro deles, as
 * sequências \{ e \} são escapes e não abrem nem fecham o comentário.
 * Espaços e comentários consecutivos são consumidos em sequência.
 *
 * Erro: comentário não fechado antes do EOF sinaliza LEX_PREMATURE_EOF.
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
					continue;
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

/*
 * isID — reconhece um identificador.
 *
 *     ID = [A-Za-z][A-Za-z0-9]*
 *
 * O texto é copiado para lexeme, truncado em MAXSTRLEN caracteres.
 * Retorna ID em caso de sucesso; caso contrário devolve o caractere
 * lido à fita e retorna 0.
 */
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

/*
 * isASGN — reconhece o operador de atribuição.
 *
 *     ASGN = ":="
 *
 * Retorna ASGN em caso de sucesso; caso contrário devolve os
 * caracteres lidos à fita e retorna 0.
 */
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

/*
 * isNUM — reconhece uma constante numérica.
 *
 *     NUM = [0-9]+ EE | [0-9]+\.[0-9]* | [0-9]*\.[0-9]+
 *
 * A leitura é delegada ao fscanf("%lg"), que aceita inteiros, decimais
 * e notação científica; o número precisa começar por um dígito. O valor
 * é regravado em lexeme com 17 dígitos significativos, preservando a
 * precisão de um double. Retorna NUM ou 0.
 */
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

/*
 * getToken — analisador léxico: entrega o próximo token da fita.
 *
 * Ignora espaços e comentários e tenta, em ordem, ID, ASGN e NUM. Se
 * nenhum padrão casar, retorna o próprio código ASCII do caractere
 * (operadores, parênteses, '\n', ';') ou EOF.
 */
int getToken(FILE *src)
{
    skipcomments(src);

    int token;
    if ((token = isID(src))) return token;
    if((token = isASGN(src))) return token;
    if ((token = isNUM(src))) return token;

    // Se nenhum padrão foi satisfeito, retorne o caracter ASCII mesmo
    token = getc(src);

    return token;
}
