CFLAGS = -I./inc -g

vbc: main.o lexer.o parser.o
	$(CC) -o $@ $^

# Targets de limpeza
clean:
	$(RM) *.o

# Deleta os arquivos de backup (normalmente produzidos pelo vim)
cleanbkp: clean
	$(RM) *~
