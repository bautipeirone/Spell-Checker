#include "checker.h"
#include "io.h"
#include "distances.h"
#include "../structures/hashtable.h"

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

int insert(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary) {
  if (len < 1)
    return 0;
  int stop = 0;
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  strcpy(buf + 1, word);
  for (unsigned i = 0; i < len + 1 && !stop; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      buf[i] = c;
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      else if (list != NULL)
        glist_add_last(list, buf, (CopyFunction) copy_str);
    }
    buf[i] = buf[i + 1];
  }
  free(buf);
  return stop;
}

int replace(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary) {
  if (len < 1)
    return 0;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  char c;
  int stop = 0;
  strcpy(buf, word);
  for (unsigned i = 0; i < len && !stop; ++i) {
    c = word[i];
    for (char x = 'a'; x < 'z'; ++x) {
      buf[i] = x < c ? x : x + 1;
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      else if (list != NULL)
        glist_add_last(list, buf, (CopyFunction) copy_str);
    }
    buf[i] = c;
  }
  free(buf);
  return stop;
}

int swap(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary) {
  if (len <= 1)
    return 0;
  int stop = 0;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  strcpy(buf, word);
  for (unsigned i = 0; i < len - 1 && !stop; ++i) {
    if (word[i] != word[i + 1]) {
      buf[i] = word[i + 1];
      buf[i + 1] = word[i];
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      else if (list != NULL)
        glist_add_last(list, buf, (CopyFunction) copy_str);
      buf[i] = word[i];
    }
  }
  free(buf);
  return stop;
}

int delete(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary) {
  if (len <= 1)
    return 0;
  int stop = 0;
  char *buf = malloc(len);
  assert(buf != NULL);
  strcpy(buf, word + 1);
  
  if (trie_search(dictionary, buf))
    stop = add_suggestion_wrongword(wword, buf);
  if (!stop && list != NULL)
    glist_add_last(list, buf, (CopyFunction) copy_str);
  
  for (unsigned i = 0; i < len - 1 && !stop; ++i) {
    buf[i] = word[i];
    if (trie_search(dictionary, buf))
      stop = add_suggestion_wrongword(wword, buf);
    if (stop)
      break;
    else if (list != NULL)
      glist_add_last(list, buf, (CopyFunction) copy_str);
  }
  free(buf);
  return stop;
}

int split(WrongWord wword, const char* word, unsigned const len, GList list, Trie dictionary) {
  if (len <= 1)
    return 0;
  int stop = 0;
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  strcpy(buf + 1, word);
  for (unsigned i = 0; i < len - 1; ++i) {
    buf[i] = buf[i + 1];
    buf[i + 1] = ' ';
    if (valid_suggestion(buf, dictionary))
      stop = add_suggestion_wrongword(wword, buf);
    if (stop)
      break;
    else if (list != NULL)
      glist_add_last(list, buf, (CopyFunction) copy_str);
  }
  free(buf);
  return stop;
}

int get_distance_1(WrongWord wword, const char* word, GList list, Trie dictionary) {
  unsigned len = strlen(word);
  if (insert(wword, word, len, list, dictionary))
    return 1;
  if (replace(wword, word, len, list, dictionary))
    return 1;
  if (swap(wword, word, len, list, dictionary))
    return 1;
  if (delete(wword, word, len, list, dictionary))
    return 1;
  if (split(wword, word, len, list, dictionary))
    return 1;
  return 0;
}

void make_suggests(WrongWord wword, Trie dictionary) {
  int stop = 0;
  HashTable tried = hashtable_init(1000UL, (CopyFunction) copy_str,
                (CompareFunction) strcmp, free, (HashFunction) KRHash);
  GList sbd[MAX_SEARCH_DISTANCE - 1]; // Suggestions by distance

  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; sbd[i++] = NULL); 

  for (int d = 1; !stop && d <= MAX_SEARCH_DISTANCE; ++d) {
    sbd[d - 1] = glist_init();
    if (d == 1) {
      // Si la distancia es 1, entonces la lista es vacia ya que no hay
      // distancias anteriores
      stop = get_distance_1(wword, wword->word, sbd[0], dictionary);
      hashtable_insert(tried, wword->word);
    } else {
      assert(sbd[d - 1] != NULL);
      // TODO: Quitar doble y triple separaciones
      for (GNode node = sbd[d - 2]->first; node != NULL; node = node->next) {
        char* top = node->data;
        if (strchr(top, ' ') != NULL)
          continue;
        if (hashtable_search(tried, top) == NULL) {
          if (d == MAX_SEARCH_DISTANCE) {
            stop = get_distance_1(wword, top, NULL, dictionary);
          } else {
            stop = get_distance_1(wword, top, sbd[d - 1], dictionary);
            hashtable_insert(tried, top);
          }
          if (stop)
            break;
        }
      }
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


// Calculo de distancias

/*(Trie root, BHeap heap, int depth,
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
    if (root->children[i] != NULL) (root->children[i], 
                        heap, depth + 1, buf, str, len, dist1);
    if (*dist1 == NUM_SUGGESTS)
      return ;
    }
  }
  return ;
}(Trie root, char *str) {
  unsigned dist1 = 0; // Keeps track of distance 1 words found
  char buf[MAX_LEN_WORD + 1];
  BHeap heap = bheap_init(100, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  // Check suggestions have the right priority in the heap
  for (int i = 0; i < NCHARS && dist1 < NUM_SUGGESTS; ++i)
    if (root->children[i](root->children[i], heap, 0, buf, str, strlen(str), &dist1);
  return heap;
}
*/