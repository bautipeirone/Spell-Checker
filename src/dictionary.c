#include "dictionary.h"
#include "io.h"

static char* copy_str(const char *str) {
  char *copy = malloc(strlen(str) + 1);
  assert(copy != NULL);
  strcpy(copy, str);
  return copy;
}

WrongWord init_wrongword(const char *str) {
  WrongWord w = malloc(sizeof(struct _WrongWord));
  assert(w != NULL);
  w->lines = queue_init();
  w->num = 0;
  w->word = copy_str(str);
  return w;
}

int cmp_wrongword(WrongWord w1, WrongWord w2) {
  return strcmp(w1->word, w2->word);
}

void free_wrongword(WrongWord w) {
  queue_free(w->lines);
  for (int i = 0; i < w->num; ++i)
    free(w->suggests[i]);
  free(w->word);
  free(w);
}

unsigned hash_wrongword(WrongWord w) {
  return KRHash(w->word);
}

int add_suggestion_wrongword(WrongWord w, char *suggestion) {
  w->suggests[w->num++] = copy_str(suggestion);
  return w->num == NUM_SUGGESTS;
}

Trie create_dictionary(const char* path) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, "No se pudo abrir el diccionario \"%s\"\n", path);
    exit(1);
  }

  char buf[MAX_LEN_WORD + 1];

  Trie root = trie_init();
  
  unsigned _line_n;
  while (read_word(fp, buf, &_line_n))
    trie_insert(root, buf);

  fclose(fp);
  return root;
}
