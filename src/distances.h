#ifndef __DIST_H__
#define __DIST_H__

#include "../utils.h"
#include "checker.h"
#include "../structures/trie.h"
#include "../structures/hashtable.h"
#include "../structures/glist.h"

// Estructura que mantiene referencia de la distancia a cierta palabra
typedef struct _WordDistance {
  char *word;
  unsigned distance;
} *WordDistance;

WordDistance init_wd(char *str, unsigned dist);

int compare_wd(WordDistance w1, WordDistance w2);

void destroy_wd(WordDistance w);

int insert(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary);

int replace(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary);

int swap(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary);

int delete(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary);

int split(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary);

int get_distance_1(WrongWord wword, const char* word, GList list, Trie dictionary);

/*
 * Calcula la cantidad de inserciones, reemplazos, eliminaciones y
 * trasposiciones para pasar de str1 a str2
*/
unsigned edit_distance(char *str1, char *str2, unsigned len1, unsigned len2);

#endif // _DIST_H__