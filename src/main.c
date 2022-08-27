#include "io.h"
#include "dictionary.h"
#include "checker.h"

int main(int argc, const char* argv[]) {
  if (argc != 3)
    usage(); 
  
  char dictionary_path[30];
  get_dict_path(dictionary_path);

  Trie dict = create_dictionary(dictionary_path); // Creacion del diccionario
  HashTable corrections = check_file(argv[1], dict); // Correcion de input
  write_corrections(argv[2], corrections); // Escritura de correcciones
  
  hashtable_free(corrections);
  trie_destroy(dict);
  return 0;
}
