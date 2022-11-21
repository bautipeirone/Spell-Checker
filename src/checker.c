#include "checker.h"
#include "io.h"
#include "distances.h"
#include "../structures/hashtable.h"

HashTable read_cachefile(const char *path) {
  FILE *fp = fopen(path, "r");
  char word_buf[MAX_LEN_WORD + 1];
  unsigned _line_n;
  HashTable cached_corrections = hashtable_init(100, id,
      (CompareFunction) cmp_wrongword, (DestroyFunction) free_wrongword,
      (HashFunction) hash_wrongword);

  while (read_word(fp, word_buf, &_line_n)) {
    WrongWord w = init_wrongword(word_buf); //lines no se utiliza en esta tabla
    w->num = read_number(fp); // Numero de sugerencias
    w->from_cache = 1;
    for (int i = 0; i < w->num; i++) {
      read_suggestion(fp, word_buf);
      w->suggests[i] = copy_str(word_buf);
    }
    hashtable_insert(cached_corrections, w);
  }

  fclose(fp);
  return cached_corrections;
}

void make_suggests(WrongWord wword, Trie dictionary) {
  int stop = 0;
  // Arreglo de tablas donde se insertaran las palabras probadas segun su
  // distancia a la palabra original. (Las de distancia 1 van en la tabla
  // attempts[0], las de distancia 2 en attempts[1], etc).
  HashTable attempts[MAX_SEARCH_DISTANCE - 1];
  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; ++i) {
    attempts[i] = hashtable_init(100U, (CopyFunction) copy_str,
      (CompareFunction) strcmp, (DestroyFunction) free, (HashFunction) KRHash);
    assert(attempts[i] != NULL);
  }
  // Se prueban todas las combinaciones de operaciones o hasta completar las
  // sugerencias pedidas. A medida que se van generando posibles palabras, se
  // agregan en varias tablas de hash para luego evitar recalcularlas. 
  stop = get_distance_1(wword, wword->word, dictionary, attempts[0], 0, NULL);
  for (unsigned d = 1; !stop && d < MAX_SEARCH_DISTANCE; ++d) {
    unsigned size = hashtable_size(attempts[d - 1]);
    char** elems = (char**) hashtable_elems(attempts[d - 1]);
    // Se recorre la tabla con palabras correspondiente a la distancia d, para
    // obtener las de distancia d + 1.
    for (unsigned i = 0; i < size; ++i) {
      char* suggest = elems[i];
      if (suggest == NULL)
        continue;
      if (d == MAX_SEARCH_DISTANCE - 1)
        // No se inserta la palabra ya que no interesa la siguiente distancia
        stop = get_distance_1(wword, suggest, dictionary, NULL, 0, NULL);
      else
        stop = get_distance_1(wword, suggest, dictionary, attempts[d], d, attempts);
      if (stop)
        break;
    }
  }
  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; ++i)
    hashtable_free(attempts[i]);
}

HashTable check_file(const char* input, Trie dictionary, HashTable cache) {
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

    // Si la palabra se encuentra en el diccionario, seguir con la proxima
    if (trie_search(dictionary, buf))
      continue;

    WrongWord w2 = hashtable_search(incorrect_words, &w1);

    /*
    Si la palabra no fue previamente corregida durante la ejecucion del
    programa, hay dos opciones:
      1 - La palabra y sus sugerencias se encuentran en el archivo intermedio
      2 - Se generan sus sugerencias
    */
    if (w2 == NULL) {
      w2 = hashtable_search(cache, &w1);
      if (w2 == NULL) {
        w2 = init_wrongword(buf);
        make_suggests(w2, dictionary);
      } else
        w2 = copy_wrongword(w2);
      hashtable_insert(incorrect_words, w2);
    }
    // Se añade la linea en que aparece
    gqueue_push(w2->lines, &line_n, (CopyFunction) copy_int);
  }

  fclose(fp);
  return incorrect_words;
}
