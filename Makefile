CFLAGS = -I./inc -g

EXEC = mybc
OUTPUT = out

$(EXEC): main.o lexer.o parser.o
	ls $(OUTPUT) || mkdir $(OUTPUT)
	$(CC) -o out/$@ $^

# Targets de limpeza
clean:
	$(RM) *.o

# Deleta os arquivos de backup (normalmente produzidos pelo vim)
cleanall: clean
	$(RM) *~
	$(RM) -rf $(OUTPUT)
