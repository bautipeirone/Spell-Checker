#ifndef __DICT_H__
#define __DICT_H__

#include "../utils.h"
#include "../structures/trie.h"
#include "../structures/gqueue.h"

typedef struct _WrongWord {
	char *word;
	char *suggests[NUM_SUGGESTS];
	int num;
  GQueue lines;
} *WrongWord;

/*
 * Inicializa una estructura WrongWord con una copia de str, y los demas campos
 * en sus valores iniciales.
*/
WrongWord init_wrongword(const char *str);

/*
 * Compara dos estructuras WrongWord. Retorna la comparacion entre las palabras
 * de cada campo.
*/
int cmp_wrongword(WrongWord w1, WrongWord w2);

/*
 * Destruye una estructura WrongWord.
*/
void free_wrongword(WrongWord w);

/*
 * Funcion de hash para una estructura WrongWord, retorna el hash de la palabra
 * dentro de la estructura
*/
unsigned hash_wrongword(WrongWord w);

/*
 * Añade una sugerencia al arreglo de sugerencias de w. Retorna 1 si ya se han
 * completado las sugerencias, 0 sino
*/
int add_suggestion_wrongword(WrongWord w, char *suggestion);

/*
 * Lee el archivo path con formato diccionario y almacena las palabras en una
 * estructura trie con el fin de tener busqueda eficiente y ahorrar memoria
 * aprovechando los prefijos que las palabras comparten
*/
Trie create_dictionary(const char* path);


#endif /* __DICT_H__ */