#ifndef __TRIE_H__
#define __TRIE_H__

#include "../utils.h"

struct TrieNode {
  char c;
  struct TrieNode *children[NCHARS];
  int end_of_word;
};

typedef struct TrieNode *Trie;

int trie_empty(Trie root);

Trie trie_init();

void trie_insert(Trie root, const char* str);

void trie_remove(Trie root, const char* str);

int trie_search(Trie root, const char* str);

void trie_destroy(Trie root);

void trie_iter(Trie root, VisitFunctionExtra visit, void *data);

#endif /* __TRIE_H__ */