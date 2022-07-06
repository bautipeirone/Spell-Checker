#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include "dictionary.h"
#include "../utils.h"
#include "../structures/hashtable.h"

/* 
 * Guarda la siguiente palabra encontrada en el archivo al buffer
 * retorna 0 si se ha finalizado la lectura del archivo, 1 en caso contrario
*/
int read_word(FILE* fp, char buf[MAX_LEN_WORD + 1], unsigned *line_number);

/*
 * Escribe al archivo del path todas las palabras a corregir con las
 * sugerencias hechas
*/
void write_corrections(const char *path, HashTable corrections);

/*
 * Imprime el uso del programa y termina el programa con codigo de error
*/
void usage();

#endif /* __IO_H__ */