#ifndef __DIST_H__
#define __DIST_H__

#include "../utils.h"
#include "checker.h"
#include "dictionary.h"
#include "../structures/trie.h"
#include "../structures/hashtable.h"

// Estructura que mantiene referencia de la distancia a cierta palabra
typedef struct _WordDistance {
  char *word;
  unsigned distance;
} *WordDistance;

WordDistance init_wd(char *str, unsigned dist);

int compare_wd(WordDistance w1, WordDistance w2);

void destroy_wd(WordDistance w);

unsigned hash_wd(WordDistance w);

int insert(WrongWord wword, WordDistance wd, unsigned const len, Trie dictionary, HashTable attempts);

int replace(WrongWord wword, WordDistance wd, unsigned const len, Trie dictionary, HashTable attempts);

int swap(WrongWord wword, WordDistance wd, unsigned const len, Trie dictionary, HashTable attempts);

int delete(WrongWord wword, WordDistance wd, unsigned const len, Trie dictionary, HashTable attempts);

int split(WrongWord wword, WordDistance wd, unsigned const len, Trie dictionary, HashTable attempts);

int get_distance_1(WrongWord wword, WordDistance wd, Trie dictionary, HashTable attempts);

/*
 * Calcula la cantidad de inserciones, reemplazos, eliminaciones y
 * trasposiciones para pasar de str1 a str2
*/
unsigned edit_distance(char *str1, char *str2, unsigned len1, unsigned len2);

#endif // _DIST_H__