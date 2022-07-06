#include "checker.h"
#include "io.h"

static inline void *id(void *p) { return p; }

static inline void lower_str(char *str) {
  for (; *str != '\0'; str++)
    *str = tolower(*str);
}

WrongWord make_suggests(Trie dictionary, char *buf) {
  (void) dictionary;
  (void) buf;
  return NULL;
}

HashTable check_file(const char* input, Trie dictionary) {
  FILE *fp = fopen(input, "r");
  assert(fp != NULL);
  char buf[MAX_LEN_WORD + 1];
  
  HashTable incorrect_words = hashtable_init(50, (CopyFunction) id,
              (CompareFunction) cmp_wrongword, (DestroyFunction) free_wrongword,
              (HashFunction) hash_wrongword);
  
  unsigned line_n = 1;

  while (read_word(fp, buf, &line_n)) {
    lower_str(buf);
    if (trie_search(dictionary, buf))
      continue;
    printf("Word not in dictionary: %s\n", buf);

    // Estructura temporal usada para buscar en la tabla hash
    struct _WrongWord w1;
    w1.word = buf;
    WrongWord w2 = hashtable_search(incorrect_words, &w1);

    if (w2 == NULL) {
      // w2 = make_suggests(dictionary, buf);
      w2 = init_wrongword(buf);
      hashtable_insert(incorrect_words, w2);
    }
    queue_push(w2->lines, line_n);
  }
  fclose(fp);
  return incorrect_words;
}
