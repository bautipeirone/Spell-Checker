#include "checker.h"
#include "io.h"
#include "distances.h"

/*void make_suggests(WrongWord word, Trie dictionary) {
  GQueue q = distance1(word->word);
  while (word->num < NUM_SUGGESTS && !gqueue_empty(q)) {
    char *tmp = gqueue_start(q, id);
    if (trie_search(dictionary, tmp)) {
      word->suggests[word->num++] = tmp;
      gqueue_pop(q, null);
    } else
      gqueue_pop(q, free);
  }
  gqueue_free(q, free);
}*/

void make_suggests(WrongWord wword, Trie dictionary) {
  BHeap distances_word = 
    calculate_distances(dictionary, wword->word, strlen(wword->word));
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

HashTable check_file(const char* input, Trie dictionary) {
  FILE *fp = fopen(input, "r");
  assert(fp != NULL);
  char buf[MAX_LEN_WORD + 1];
  
  // Estructura temporal usada para buscar en la tabla hash
  struct _WrongWord w1;
  w1.word = buf;
  
  HashTable incorrect_words = hashtable_init(500, (CopyFunction) id,
              (CompareFunction) cmp_wrongword, (DestroyFunction) free_wrongword,
              (HashFunction) hash_wrongword);
  
  unsigned line_n = 1;

  while (read_word(fp, buf, &line_n)) {
    lower_str(buf);

    if (trie_search(dictionary, buf))
      continue;

    WrongWord w2 = hashtable_search(incorrect_words, &w1);

    if (w2 == NULL) {
      w2 = init_wrongword(buf);
      make_suggests(w2, dictionary);
      hashtable_insert(incorrect_words, w2);
    }
    queue_push(w2->lines, line_n);
  }

  fclose(fp);
  return incorrect_words;
}

int __calculate_distances(Trie root, BHeap heap, int depth,
              char buf[MAX_LEN_WORD + 1], char *str, int len, unsigned *dist1) {
  buf[depth] = root->c;
  if (root->end_of_word && (len - depth) <= MAX_SEARCH_DISTANCE) {
    unsigned distance = edit_distance(str, buf, len, depth + 1);
    buf[depth + 1] = '\0';
    bheap_insert(heap, init_wd(buf, distance));
    if (distance == 1)
      if (++(*dist1) == MAX_SEARCH_DISTANCE)
        return 1;
  }

  if (depth - len > MAX_SEARCH_DISTANCE)
    return 0;

  int stop = 0;
  for (int i = 0; i < NCHARS; ++i) {
    if (root->children[i] != NULL)
      stop = __calculate_distances(root->children[i], 
                        heap, depth + 1, buf, str, len, dist1);
    if (stop)
      return 1;
  }
  return 0;
}

BHeap calculate_distances(Trie root, char *str, int len) {
  unsigned dist1 = 0; // Keeps track of distance 1 words found
  char buf[MAX_LEN_WORD + 1];
  BHeap heap = bheap_init(100, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  // Check suggestions have the right priority in the heap
  for (int i = 0; i < NCHARS; ++i)
    if (root->children[i])
      __calculate_distances(root->children[i], heap, 0, buf, str, len, &dist1);
  return heap;
}
