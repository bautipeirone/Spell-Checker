#ifndef __DIST_H__
#define __DIST_H__

#include "../utils.h"
#include "checker.h"
#include "dictionary.h"
#include "../structures/trie.h"
#include "../structures/hashtable.h"

/*
 * Regla de insercion: Se prueban todas las posibles inserciones de un caracter
 * en str. Si es valida, se añade como sugerencia a wword
 * Retorna 1 si se han completado las sugerencias, 0 en caso contrario
*/
int insert(WrongWord wword, char* str, unsigned len, Trie dictionary,
              HashTable attempts);//, unsigned dist, HashTable prev_attempts[]);

/*
 * Regla de reemplazo: Todos los caracteres de str son reemplazados por otro
 * distinto. Si es una palabra valida, se añade como sugerencia a wword
 * Retorna 1 si se han completado las sugerencias, 0 en caso contrario
*/
int replace(WrongWord wword, char* str, unsigned len, Trie dictionary,
              HashTable attempts);//, unsigned dist, HashTable prev_attempts[]);

/*
 * Regla de trasposicion: Se alternan los caracteres adyacentes de la palabra
 * Si es una palabra valida, se añade como sugerencia a wword
 * Retorna 1 si se han completado las sugerencias, 0 en caso contrario
*/
int swap(WrongWord wword, char* str, unsigned len, Trie dictionary,
              HashTable attempts);//, unsigned dist, HashTable prev_attempts[]);

/*
 * Regla de eliminacion: Se elimina un caracter de la palabra. Si es una palabra
 * valida, se añade como sugerencia a wword
 * Retorna 1 si se han completado las sugerencias, 0 en caso contrario
*/
int delete(WrongWord wword, char* str, unsigned len, Trie dictionary,
              HashTable attempts);//, unsigned dist, HashTable prev_attempts[]);

/*
 * Regla de separacion: Se divide el string en dos substrings separados por un
 * espacio. Si es ambos substring son validos, se añade como sugerencia a wword
 * Retorna 1 si se han completado las sugerencias, 0 en caso contrario
*/
int split(WrongWord wword, char* str, unsigned len, Trie dictionary);

/*
 * Calcula todas las sugerencias de distancia 1 de str, o hasta que se completen
 * las sugerencias. Todas las reglas añaden los intentos hechos a una tabla para
 * luego ser iterada y evitar recalcular estas
*/
int get_distance_1(WrongWord wword, char* str, Trie dictionary,
              HashTable attempts);//, unsigned dist, HashTable prev_attempts[]);

#endif // __DIST_H__