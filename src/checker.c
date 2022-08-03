#include "checker.h"
#include "io.h"
#include "distances.h"

void make_suggests(WrongWord wword, HashTable dictionary, DTree dist_dict) {
  (void) dictionary;
  BHeap distances_word =
    dtree_search_by_distance(dist_dict, wword->word, MAX_SEARCH_DISTANCE);
  while (wword->num < NUM_SUGGESTS) {
    WordDistance suggest = bheap_remove_max(distances_word);
    if (suggest == NULL)
      break;
    if (suggest->distance <= MAX_SEARCH_DISTANCE)
      add_suggestion_wrongword(wword, suggest->word);
    destroy_wd(suggest);
  }
  bheap_destroy(distances_word);
}

HashTable check_file(const char* input, HashTable dictionary, DTree dist_dict) {
  FILE *fp = fopen(input, "r");
  assert(fp != NULL);
  char buf[MAX_LEN_WORD + 1];
  
  // Estructura temporal usada para buscar en la tabla hash
  struct _WrongWord w1;
  w1.word = buf;
  
  HashTable incorrect_words = hashtable_init(10, (CopyFunction) id,
              (CompareFunction) cmp_wrongword, (DestroyFunction) free_wrongword,
              (HashFunction) hash_wrongword);
  
  unsigned line_n = 1;

  while (read_word(fp, buf, &line_n)) {
    lower_str(buf);

    if (hashtable_search(dictionary, buf))
      continue;

    WrongWord w2 = hashtable_search(incorrect_words, &w1);

    if (w2 == NULL) {
      w2 = init_wrongword(buf);
      make_suggests(w2, dictionary, dist_dict);
      hashtable_insert(incorrect_words, w2);
    }
    queue_push(w2->lines, line_n);
  }

  fclose(fp);
  return incorrect_words;
}
