#include "trie.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

__attribute__((unused))
static inline unsigned index_of_char(char c) { return c - 'a'; }

int trie_empty(Trie root) { return root == NULL; }

Trie trie_init() {
  Trie node = malloc(sizeof(struct TrieNode));
  assert(node != NULL);
  node->c = '\0';
  node->end_of_word = 0;
  for (int i = 0; i < NCHARS; ++i)
    node->children[i] = NULL;
  return node;
}

void trie_insert(Trie root, const char* str) {
  assert(!trie_empty(root));
  Trie tmp;
  for (tmp = root; *str != '\0'; str++) {
    if (!('a' <= *str && *str <= 'z')) {
      putchar(*str);
      return;
    }
    if (tmp->children[*str - 'a'] == NULL) {
      tmp->children[*str - 'a'] = trie_init();
      tmp->children[*str - 'a']->c = *str;
    }
    tmp = tmp->children[*str - 'a'];
  }
  tmp->end_of_word = 1;
}

int trie_search(Trie root, const char* str) {
  if (trie_empty(root))
    return 0;
  Trie tmp;
  for (tmp = root; *str != '\0'; str++) {
    // La cadena no se encuentra en el trie
    if (tmp->children[*str - 'a'] == NULL)
      return 0;
    tmp = tmp->children[*str - 'a'];
  }
  // Si la cadena esta en el trie, queda ver
  // si es una palabra valida o no
  return tmp->end_of_word;
}

void trie_remove(Trie root, const char* str) {
  if (trie_empty(root))
    return;
  Trie tmp;
  for (tmp = root; *str != '\0'; str++) {
    // La cadena no se encuentra en el trie
    if (tmp->children[*str - 'a'] == NULL)
      return;
    tmp = tmp->children[*str - 'a'];
  }
  tmp->end_of_word = 0;
}

void trie_destroy(Trie root) {
  if (trie_empty(root))
    return;
  for (int i = 0; i < NCHARS; ++i)
    trie_destroy(root->children[i]);
  free(root);
}

void trie_iter_impl(Trie root, int depth, char buf[MAX_LEN_WORD], VisitFunctionExtra visit, void *data) {
  buf[depth] = root->c;
  if (root->end_of_word) {
    buf[depth + 1] = '\0';
    visit(buf, data);
  }
  for (int i = 0; i < NCHARS; ++i) {
    if (root->children[i] != NULL)
      trie_iter_impl(root->children[i], depth + 1, buf, visit, data);
  }
}

void trie_iter(Trie root, VisitFunctionExtra visit, void *data) {
  char buf[MAX_LEN_WORD + 1];
  for (int i = 0; i < NCHARS; ++i)
    if (root->children[i])
      trie_iter_impl(root->children[i], 0, buf, visit, data);
}
