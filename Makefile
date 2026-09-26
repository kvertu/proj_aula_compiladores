CFLAGS = -I./inc -g

EXEC = mybc

$(EXEC): main.o lexer.o parser.o
	$(CC) -o $@ $^

# Targets de limpeza
clean:
	$(RM) *.o

# Deleta os arquivos de backup (normalmente produzidos pelo vim)
cleanall: clean
	$(RM) *~ $(EXEC)
