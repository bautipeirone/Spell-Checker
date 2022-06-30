#include "dictionary.h"
#include <string.h>
#include <assert.h>

Trie create_dictionary(const char* path) {

  FILE* fp = fopen(path, "r");
  assert(fp != NULL);
  char buf[MAX_LEN_WORD];

  Trie root = trie_init();
  int c;
  while ((c = getc(fp))) {
    if (c == EOF)

    trie_insert(root, buf);
  }

  fclose(fp);
  return root;
}