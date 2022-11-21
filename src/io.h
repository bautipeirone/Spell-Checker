#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include "dictionary.h"
#include "../utils.h"
#include "../structures/hashtable.h"

/*
 * Retorna el nombre del archivo del diccionario
*/
void get_dict_path(char path[]);

/* 
 * Guarda la siguiente palabra encontrada en el archivo al buffer,
 * retorna 0 si se ha finalizado la lectura del archivo, 1 en caso contrario
*/
int read_word(FILE* fp, char buf[], unsigned *line_number);

/*
 * Retorna el siguiente numero encontrado en el archivo. Se espera
 * que lo proximo a leer sean espacios, comas o caracteres numericos
*/
int read_number(FILE *fp);

/*
 * Lee una sugerencia del archivo. La diferencia principal con read_word es que
 * una sugerencia puede contener caracteres de espacio, mientras que read_word
 * entiende esto como palabras distintas.
 * read_suggestion siempre asume por simplicidad que hay una proxima palabra
 * por leer
*/
void read_suggestion(FILE *fp, char buf[]);


/*
 * Escribe al archivo del path todas las palabras a corregir con las
 * sugerencias hechas
*/
void write_corrections(const char *path, HashTable corrections);


/*
 * Escribe una linea en el archivo intermedio si la palabra no se
 * hayaba ya corregida en el archivo
*/
void add_to_cachefile(WrongWord w, FILE *fp);

/*
 * Actualiza el archivo intermedio con las nuevas sugerencias hechas
*/
void update_cachefile(const char *path, HashTable corrections);

/*
 * Imprime el uso del programa y termina el programa con codigo de error
*/
void usage();

#endif /* __IO_H__ */
