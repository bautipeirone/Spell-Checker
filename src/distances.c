#include "distances.h"

static int valid_suggestion(char* word, Trie dictionary) {
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


static inline int min(const int x, const int y) {
  if (x > y)
    return y;
  return x;
}

__attribute__((unused))
static inline int min3(int x, int y, int z) {
  return min(min(x, y), z);
}

__attribute__((unused))
static inline int min4(const int x, const int y, const int z, const int w) {
  return min(min(x, y), min(z,w));
}

WordDistance init_wd(char *str, unsigned dist) {
  WordDistance w = malloc(sizeof(struct _WordDistance));
  assert(w != NULL);
  w->word = copy_str(str);
  w->distance = dist;
  return w;
}

int compare_wd(WordDistance w1, WordDistance w2) {
  // Aquel con distancia mayor tiene menor prioridad
  return w2->distance - w1->distance;
}

void destroy_wd(WordDistance w) {
  free(w->word);
  free(w);
}

unsigned hash_wd(WordDistance w) {
  return KRHash(w->word);
}

/*            OPERACIONES DE EDICION DE CADENAS            */
/* ####################################################### */

int insert(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts, unsigned dist, HashTable prev_attempts[]) {
  if (len < 1)
    return 0;
  int stop = 0, insert_flag = 1;
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  buf[len + 1] = '\0';
  strcpy(buf + 1, str);
  for (unsigned i = 0; i < len + 1 && !stop; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      buf[i] = c;
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;
      if (insert_flag && attempts != NULL)
        hashtable_insert(attempts, buf);
    }
    buf[i] = buf[i + 1];
  }
  free(buf);
  return stop;
}

int replace(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts, unsigned dist, HashTable prev_attempts[]) {
  if (len < 1)
    return 0;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  buf[len] = '\0';
  char c;
  int stop = 0, insert_flag = 1;
  strcpy(buf, str);
  for (unsigned i = 0; i < len && !stop; ++i) {
    c = str[i];
    for (char x = 'a'; x < 'z'; ++x) {
      buf[i] = x < c ? x : x + 1;
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      if (stop)
        break;
      for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;
      if (insert_flag && attempts != NULL)
        hashtable_insert(attempts, buf);
    }
    buf[i] = c;
  }
  free(buf);
  return stop;
}

int swap(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts, unsigned dist, HashTable prev_attempts[]) {
  if (len <= 1)
    return 0;
  int stop = 0, insert_flag = 1;
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  buf[len] = '\0';
  strcpy(buf, str);
  for (unsigned i = 0; i < len - 1 && !stop; ++i) {
    if (str[i] != str[i + 1]) {
      buf[i] = str[i + 1];
      buf[i + 1] = str[i];
      if (trie_search(dictionary, buf))
        stop = add_suggestion_wrongword(wword, buf);
      for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;
      if (insert_flag && attempts != NULL)
        hashtable_insert(attempts, buf);
      buf[i] = str[i];
    }
  }
  free(buf);
  return stop;
}

int delete(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts, unsigned dist, HashTable prev_attempts[]) {
  if (len <= 1)
    return 0;
  int stop = 0, insert_flag = 1;
  char *buf = malloc(len);
  assert(buf != NULL);
  buf[len - 1] = '\0';
  strcpy(buf, str + 1);
  unsigned i = 0;
  do {
    if (trie_search(dictionary, buf))
      stop = add_suggestion_wrongword(wword, buf);
    for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;
    if (insert_flag && attempts != NULL)
      hashtable_insert(attempts, buf);
    buf[i] = str[i];
    ++i;
  } while (i < len && !stop);
  
  free(buf);
  return stop;
}

int split(WrongWord wword, char* str, unsigned len, Trie dictionary,
        HashTable attempts, unsigned dist, HashTable prev_attempts[]) {
  if (len <= 1)
    return 0;
  int stop = 0, insert_flag = 1;
  
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  buf[len + 1] = '\0';
  strcpy(buf + 1, str);

  for (unsigned i = 0; i < len - 1 && !stop; ++i) { // (ab cd) => 5 sugerencias => stop = 1, (abc d) => 6 sugerencias => stop = 0
    buf[i] = buf[i + 1];
    buf[i + 1] = ' ';
    if (valid_suggestion(buf, dictionary))
      stop = add_suggestion_wrongword(wword, buf);
    for (unsigned j = 0; j < dist; ++j)
        // La palabra ya fue agregada anteriormente, por lo que no es necesario
        // agregarla a la tabla con sugerencia con la distancia actual
        if (hashtable_search(prev_attempts[j], buf))
          insert_flag = 0;
    if (insert_flag && attempts != NULL)
      hashtable_insert(attempts, buf);
  }
  free(buf);
  return stop;
}

int get_distance_1(WrongWord wword, char* str, Trie dictionary,
              HashTable attempts, unsigned dist, HashTable prev_attempts[]) {
  unsigned len = strlen(str);
  if (insert(wword, str, len, dictionary, attempts, dist, prev_attempts))
    return 1;
  if (replace(wword, str, len, dictionary, attempts, dist, prev_attempts))
    return 1;
  if (swap(wword, str, len, dictionary, attempts, dist, prev_attempts))
    return 1;
  if (delete(wword, str, len, dictionary, attempts, dist, prev_attempts))
    return 1;
  if (split(wword, str, len, dictionary, attempts, dist, prev_attempts))
    return 1;
  return 0;
}

/* ####################################################################### */

/*
unsigned edit_distance(char *str1, char *str2, unsigned len1, unsigned len2) {
  // Memoria O(m), m = len2
  // Tiempo O(n * m), n = len1, m = len2
  
  // Arreglo donde cada caracter tiene asociado la ultima fila donde aparece
  unsigned last_row[NCHARS], last_col, cost;

  for (unsigned i = 0; i < NCHARS; last_row[i++] = 1);
  
  unsigned **matrix = malloc(sizeof(unsigned*) * (len1 + 2));
  assert(matrix != NULL);
  for (unsigned nrow = 0; nrow < len1 + 2; ++nrow) {
    matrix[nrow] = malloc(sizeof(unsigned) * (len2 + 2));
    assert(matrix[nrow] != NULL);
  }

  matrix[0][0] = matrix[1][0] = 10 * MAX_SEARCH_DISTANCE;
  for (unsigned j = 1; j < len2 + 2; ++j) {
    matrix[0][j] = 10 * MAX_SEARCH_DISTANCE;
    matrix[1][j] = j - 1;
  }

  for (unsigned i = 2; i < len1 + 2; ++i) {
    last_col = 1;
    
    // Inicializamos los dos primeros datos de la fila
    // con sus correspondientes valores
    matrix[i][0] = 10 * MAX_SEARCH_DISTANCE;
    matrix[i][1] = i - 1;
    
    for (unsigned j = 2; j < len2 + 2; ++j) {
      unsigned k = last_row[str2[j - 2] - 'a'];
      unsigned s = last_col;
      cost = 1;
      if (str1[i - 2] == str2[j - 2]) {
        cost = 0;
        last_col = j;
      }
      matrix[i][j] = min4(
        matrix[i - 1][j - 1] + cost, matrix[i][j - 1] + 1, matrix[i - 1][j] + 1,
        matrix[k - 1][s - 1] + (i - k - 1) + (j - s - 1) + 1);
    }
    last_row[str1[i - 2] - 'a'] = i;
  }

  unsigned result = matrix[len1 + 1][len2 + 1];
  for (unsigned i = 0; i < len1 + 2; free(matrix[i++]));
  free(matrix);

  return result;
}

unsigned edit_distance(char *str1, char *str2, unsigned len1, unsigned len2) {
  unsigned *distances = malloc(sizeof(unsigned) * (len2 + 1));
  unsigned *buffer = malloc(sizeof(unsigned) * (len2 + 1));
  
  for (unsigned i = 0; i <= len2; ++i)
    distances[i] = i;
  for (unsigned i = 0; i < len1; ++i) {
    buffer[0] = i + 1;
    for (unsigned j = 1; j < len2 + 1; ++j) {
      unsigned cost = str1[i] == str2[j - 1] ? 0 : 1;
      buffer[j] = 
        min3(buffer[j - 1] + 1, distances[j] + 1, distances[j - 1] + cost);
    }
    unsigned *tmp = distances;
    distances = buffer;
    buffer = tmp;
  }
  unsigned result = distances[len2];
  free(distances);
  free(buffer);

  return result;
}
*/