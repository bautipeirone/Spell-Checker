#ifndef __TRIE_H__
#define __TRIE_H__

#include "../utils.h"

struct TrieNode {
  char c;                             // Caracter que representa el nodo
  struct TrieNode *children[NCHARS];  // Arreglo de hijos
  char end_of_word;                   // Caracter usado como bool, indica si
};                                    // el nodo representa una palabra completa

typedef struct TrieNode *Trie;

int trie_empty(Trie root);

Trie trie_init(char c);

void trie_insert(Trie root, const char* str);

void trie_remove(Trie root, const char* str);

int trie_search(Trie root, const char* str);

void trie_destroy(Trie root);

void trie_iter(Trie root, VisitFunctionExtra visit, void *data);

#endif /* __TRIE_H__ */