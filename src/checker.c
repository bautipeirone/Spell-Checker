#include "checker.h"
#include "io.h"
#include "distances.h"
#include "../structures/hashtable.h"

void insert(char *word, unsigned const len, GList list) {
  if (len < 1)
    return;
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  strcpy(buf + 1, word);
  for (unsigned i = 0; i < len + 1; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      buf[i] = c;
      glist_add_last(list, buf, (CopyFunction) copy_str);
    }
    buf[i] = buf[i + 1];
  }
  free(buf);
}

void replace(char *word, unsigned const len, GList list) {
  if (len < 1)
    return;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  char c;
  strcpy(buf, word);
  for (unsigned i = 0; i < len; ++i) {
    c = word[i];
    for (char x = 'a'; x < 'z'; ++x) {
      buf[i] = x < c ? x : x + 1;
      glist_add_last(list, buf, (CopyFunction) copy_str);
    }
    buf[i] = c;
  }
  free(buf);
}

void swap(char *word, unsigned const len, GList list) {
  if (len <= 1)
    return;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  strcpy(buf, word);
  for (unsigned i = 0; i < len - 1; ++i) {
    if (word[i] != word[i + 1]) {
      buf[i] = word[i + 1];
      buf[i + 1] = word[i];
      glist_add_last(list, buf, (CopyFunction) copy_str);
      buf[i] = word[i];
    }
  }
  free(buf);
}

void delete(char *word, unsigned const len, GList list) {
  if (len <= 1)
    return;
  char *buf = malloc(len);
  assert(buf != NULL);
  strcpy(buf, word + 1);
  glist_add_last(list, buf, (CopyFunction) copy_str);
  for (unsigned i = 0; i < len - 1; ++i) {
    buf[i] = word[i];
    glist_add_last(list, buf, (CopyFunction) copy_str);
  }
  free(buf);
}

void split(char *word, unsigned const len, GList list) {
  if (len <= 1)
    return;
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  strcpy(buf + 1, word);
  for (unsigned i = 0; i < len - 1; ++i) {
    buf[i] = buf[i + 1];
    buf[i + 1] = ' ';
    glist_add_last(list, buf, (CopyFunction) copy_str);
  }
  free(buf);
}

void distance1(char *word, GList list, int calculate_split) {
  unsigned len = strlen(word);
  insert(word, len, list);
  replace(word, len, list);
  swap(word, len, list);
  delete(word, len, list);
  if (calculate_split)
    split(word, len, list);
}

/*
void make_suggests(WrongWord word, Trie dictionary) {
  GQueue list = distance1(word->word);
  while (word->num < NUM_SUGGESTS && !gqueue_empty(list)) {
    char *tmp = gqueue_start(list, id);
    if (trie_search(dictionary, tmp)) {
      word->suggests[word->num++] = tmp;
      gqueue_pop(list, null);
    } else
      gqueue_pop(list, free);
  }
  gqueue_free(list, free);
}
*/

/*
void make_suggests_distances(WrongWord wword, Trie dictionary) {
  unsigned last_dist = 1;
  BHeap distances_word = 
    calculate_distances(dictionary, wword->word);
  add_split_suggests(wword, distances_word, dictionary);

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

void make_suggests(WrongWord wword, Trie dictionary) {
  if (strlen(wword->word) > 12)
    make_suggests_distances(wword, dictionary);
  else
    make_suggests_bf(wword, dictionary);
}
*/

int valid_suggestion(char* word, Trie dictionary) {
  int result;
  char* copy = copy_str(word); // TODO: remove copy
  char* space = strchr(copy, ' ');
  if (space == NULL) // La sugerencia consta de una sola palabra
    result = trie_search(dictionary, word);
  else {
    *space = '\0';
    result = trie_search(dictionary, copy) && trie_search(dictionary, space + 1);
    *space = ' ';
  }
  free(copy);
  return result;
}

GList __brute_force(char* word, unsigned distance, GList list, HashTable __attribute__((unused)) tried, int calculate_split) {
  GList list2 = glist_init();
  if (distance == 1) {
    // Si la distancia es 1, entonces la lista es vacia ya que no hay
    // distancias anteriores
    distance1(word, list2, calculate_split);
  } else {
    assert(list != NULL);
    // TODO: Quitar doble y triple separaciones
    for (GNode node = list->first; node != NULL; node = node->next) {
      char* top = node->data;
      if (strchr(top, ' ') != NULL)
        continue;
      if (hashtable_search(tried, top) == NULL) {
        distance1(top, list2, calculate_split);
        hashtable_insert(tried, top);
      }
    }
  }
  return list2;
}

GList brute_force(char* word, unsigned distance, GList previous_distances, HashTable tried) {
  assert(distance > 0);
  return __brute_force(word, distance, previous_distances, tried, 1);
}

void make_suggests(WrongWord wword, Trie dictionary) {
  char* suggestion;
  int stop = 0;
  HashTable tried = hashtable_init(1000UL, (CopyFunction) copy_str,
                (CompareFunction) strcmp, free, (HashFunction) KRHash);
  GList sbd[MAX_SEARCH_DISTANCE - 1]; // Suggestions by distance
  // TODO: Initialize before loop and passed them as buffer
  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; sbd[i++] = NULL); 

  for (int i = 1; !stop && i <= MAX_SEARCH_DISTANCE; ++i) {
    sbd[i - 1] = brute_force(wword->word, i, (i > 1 ? sbd[i - 2] : NULL), tried);
    for (GNode node = sbd[i - 1]->first; !stop && node != NULL; node = node->next) {
      suggestion = (char*) node->data;
      if (valid_suggestion(suggestion, dictionary))
        stop = add_suggestion_wrongword(wword, suggestion);
    }
  }
  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; glist_free(sbd[i++], free));
  hashtable_free(tried);
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
    gqueue_push(w2->lines, &line_n, (CopyFunction) copy_int);
  }

  fclose(fp);
  return incorrect_words;
}

void __calculate_distances(Trie root, BHeap heap, int depth,
              char buf[MAX_LEN_WORD + 1], char *str, int len, unsigned *dist1) {
  buf[depth] = root->c;
  if (root->end_of_word && (len - depth) <= MAX_SEARCH_DISTANCE) {
    unsigned distance = edit_distance(str, buf, len, depth + 1);
    buf[depth + 1] = '\0';
    bheap_insert(heap, init_wd(buf, distance));
    if (distance == 1)
      if (++(*dist1) == NUM_SUGGESTS)
        return ;
    }

  if (depth - len > MAX_SEARCH_DISTANCE)
    return ;

  for (int i = 0; i < NCHARS; ++i) {
    if (root->children[i] != NULL) {
      __calculate_distances(root->children[i], 
                        heap, depth + 1, buf, str, len, dist1);
    if (*dist1 == NUM_SUGGESTS)
      return ;
    }
  }
  return ;
}

BHeap calculate_distances(Trie root, char *str) {
  unsigned dist1 = 0; // Keeps track of distance 1 words found
  char buf[MAX_LEN_WORD + 1];
  BHeap heap = bheap_init(100, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  // Check suggestions have the right priority in the heap
  for (int i = 0; i < NCHARS && dist1 < NUM_SUGGESTS; ++i)
    if (root->children[i])
      __calculate_distances(root->children[i], heap, 0, buf, str, strlen(str), &dist1);
  return heap;
}
