#ifndef __TRIE_H__
#define __TRIE_H__

#include "../utils.h"

struct TrieNode {
  char c;                             // Caracter que representa el nodo
  struct TrieNode *children[NCHARS];  // Arreglo de hijos
  char end_of_word;                   // Caracter usado como bool, indica si
};                                    // el nodo representa una palabra completa

typedef struct TrieNode *Trie;

/*
 * Determina si un Trie esta vacio
*/
int trie_empty(Trie root);

/*
 * Crea un arbol vacio
*/
Trie trie_init(char c);

/*
 * Inserta un string en un Trie
*/
void trie_insert(Trie root, const char* str);

/*
 * Remueve un string de un Trie
*/
void trie_remove(Trie root, const char* str);

/*
 * Busca un string en el Trie
*/
int trie_search(Trie root, const char* str);

/*
 * Libera el Trie
*/
void trie_destroy(Trie root);

/*
 * Recorre el Trie, aplicando la funcion a todas las cadenas del Trie
*/
void trie_iter(Trie root, VisitFunctionExtra visit, void *data);

#endif /* __TRIE_H__ */