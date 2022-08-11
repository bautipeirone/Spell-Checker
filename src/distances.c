#include "distances.h"


static inline int min(const int x, const int y) {
  if (x > y)
    return y;
  return x;
}

__attribute__((unused))
static inline int min3(int x, int y, int z) {
  return min(min(x, y), z);
}

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

/*
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