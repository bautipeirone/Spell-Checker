/*
void make_suggests_distances(WrongWord wword, Trie dictionary) {
  unsigned last_dist = 1;
  BHeap distances_word =(dictionary, wword->word);
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

GList __brute_force(char* word, unsigned distance, GList list, HashTable __attribute__((unused)) tried) {
  GList list2 = glist_init();
  
  return list2;
}

GList brute_force(char* word, unsigned distance, GList previous_distances, HashTable tried) {
  assert(distance > 0);
  return __brute_force(word, distance, previous_distances, tried, 1);
}

GList brute_force(Trie dictionary, char* word, GList list,
                  Trie repeated_words) {
  if (max_distance == MAX_)
    return NULL;
  else if (max_distance == 1)
    return distance1(word);
}

*/