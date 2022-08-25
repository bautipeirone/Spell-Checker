#include "distances.h"

static int valid_split(char* word, Trie dictionary) {
  int result;
  char* space = strchr(word, ' ');
  *space = '\0';
  result = trie_search(dictionary, word) && trie_search(dictionary, space + 1);
  *space = ' ';
  return result;
}

/*            OPERACIONES DE EDICION DE CADENAS            */
/* ####################################################### */

int insert(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts) {// , unsigned dist, HashTable prev_attempts[]) {
  if (len < 1)
    return 0;
  int stop = 0; //, insert_flag = 1;
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  buf[len + 1] = '\0';
  memcpy(buf + 1, str, len + 1);
  for (unsigned i = 0; i < len + 1 && !stop; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      buf[i] = c;
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      /*for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;
      */
      if (attempts != NULL)
        hashtable_insert(attempts, buf);
    }
    buf[i] = buf[i + 1];
  }
  free(buf);
  return stop;
}

int replace(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts) {//, unsigned dist, HashTable prev_attempts[]) {
  if (len < 1)
    return 0;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  buf[len] = '\0';
  char c;
  int stop = 0;//, insert_flag = 1;
  memcpy(buf, str, len + 1);
  for (unsigned i = 0; i < len && !stop; ++i) {
    c = str[i];
    for (char x = 'a'; x < 'z'; ++x) {
      buf[i] = x < c ? x : x + 1;
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      /*for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;*/
      if (attempts != NULL)
        hashtable_insert(attempts, buf);
    }
    buf[i] = c;
  }
  free(buf);
  return stop;
}

int swap(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts) {//, unsigned dist, HashTable prev_attempts[]) {
  if (len <= 1)
    return 0;
  int stop = 0;//, insert_flag = 1;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  buf[len] = '\0';
  memcpy(buf, str, len + 1);
  for (unsigned i = 0; i < len - 1 && !stop; ++i) {
    if (str[i] != str[i + 1]) {
      buf[i] = str[i + 1];
      buf[i + 1] = str[i];
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      /*for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;*/
      if (attempts != NULL)
        hashtable_insert(attempts, buf);
      buf[i] = str[i];
    }
  }
  free(buf);
  return stop;
}

int delete(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts) {//, unsigned dist, HashTable prev_attempts[]) {
  if (len <= 1)
    return 0;
  int stop = 0;//, insert_flag = 1;
  char *buf = malloc(len);
  assert(buf != NULL);
  buf[len - 1] = '\0';
  memcpy(buf, str + 1, len);
  unsigned i = 0;
  do {
    if (trie_search(dictionary, buf))
      stop = add_suggestion_wrongword(wword, buf);
    /*for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;*/
    if (attempts != NULL)
      hashtable_insert(attempts, buf);
    buf[i] = str[i];
    ++i;
  } while (i < len && !stop);
  
  free(buf);
  return stop;
}

int split(WrongWord wword, char* str, unsigned len, Trie dictionary) {
  if (len <= 1)
    return 0;
  int stop = 0;//, insert_flag = 1;
  
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  buf[len + 1] = '\0';
  memcpy(buf + 1, str, len + 1);

  for (unsigned i = 0; i < len - 1 && !stop; ++i) {
    buf[i] = buf[i + 1];
    buf[i + 1] = ' ';
    if (valid_split(buf, dictionary))
      stop = add_suggestion_wrongword(wword, buf);
  }
  free(buf);
  return stop;
}

int get_distance_1(WrongWord wword, char* str, Trie dictionary,
              HashTable attempts) {// ) {
  unsigned len = strlen(str);
  if (insert(wword, str, len, dictionary, attempts)) //, dist, prev_attempts))
    return 1;
  if (replace(wword, str, len, dictionary, attempts)) //, dist, prev_attempts))
    return 1;
  if (swap(wword, str, len, dictionary, attempts)) //, dist, prev_attempts))
    return 1;
  if (delete(wword, str, len, dictionary, attempts)) //, dist, prev_attempts))
    return 1;
  if (split(wword, str, len, dictionary))
    return 1;
  return 0;
}
