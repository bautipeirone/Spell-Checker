# Makefile para corrector ortografico

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
BIN = sc

# Archivos fuente
MAIN = src/main
DICTIONARY = src/dictionary
CHECKER = src/checker
#IO = src/io

HASHTABLE = structures/hashtable
TRIE = structures/trie

SOURCE = $(DICTIONARY).o $(CHECKER).o
STRUCTURES = $(HASHTABLE).o $(TRIE).o
TESTS = tests/main.c 

# Compilar el programa
all: $(MAIN).o $(SOURCE) $(STRUCTURES)
	$(CC) $(CFLAGS) $^ -o $(BIN)

# Crear .o desde archivos .c
%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Compilar para depuracion
.PHONY: debug
debug:
	$(CC) -g $(CFLAGS) $(MAIN).c $(DICTIONARY).c \
	$(CHECKER).c $(HASHTABLE).c $(TRIE).c -o $(BIN)

# Remover archivos .o, ejecutables, etc
.PHONY: clean
clean:
	@-rm -r *.dSYM 2>/dev/null || true
	@-rm src/*.o structures/*.o 2>/dev/null || true
	@-rm $(BIN) 2>/dev/null || true 
