#include "dictionary.h"
#include "io.h"

WrongWord init_wrongword(const char *str) {
  WrongWord w = malloc(sizeof(struct _WrongWord));
  assert(w != NULL);
  w->lines = gqueue_init();
  w->num = 0;
  w->word = copy_str(str);
  return w;
}

inline int cmp_wrongword(WrongWord w1, WrongWord w2) {
  return strcmp(w1->word, w2->word);
}

void free_wrongword(WrongWord w) {
  gqueue_free(w->lines, free);
  for (int i = 0; i < w->num; ++i)
    free(w->suggests[i]);
  free(w->word);
  free(w);
}

inline unsigned hash_wrongword(WrongWord w) {
  return KRHash(w->word);
}

int add_suggestion_wrongword(WrongWord w, char *suggestion) {
  int stop = 0;
  for (int i = 0; i < w->num && !stop; ++i)
    stop = !strcmp(w->suggests[i], suggestion);
  // Si la sugerencia no fue previamente hecha, se añade como nueva
  if (!stop) {
    w->suggests[w->num++] = copy_str(suggestion);
    return w->num == NUM_SUGGESTS;
  } else
    return 0;
}

Trie create_dictionary(const char* path) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, "No se pudo abrir el diccionario \"%s\"\n", path);
    exit(EXIT_FAILURE);
  }

  char buf[MAX_LEN_WORD + 1];

  unsigned _len;
  Trie root = trie_init('\0');

  // Palabra por palabra se crea el diccionario en una estructura trie
  while (read_word(fp, buf, &_len))
    trie_insert(root, buf);

  fclose(fp);
  return root;
}
