#include "io.h"
#include "dictionary.h"
#include "checker.h"

int main(int argc, const char* argv[]) {
  if (argc != 4)
    usage(); 
  
  char dictionary_path[30];
  get_dict_path(dictionary_path);

  Trie dict = create_dictionary(dictionary_path); // Creacion del diccionario
  // Se cargan las correciones ya hechas en el archivo intermedio
  HashTable cache = read_cachefile(argv[3]);
  HashTable corrections = check_file(argv[1], dict, cache);// Correcion de input
  write_corrections(argv[2], corrections); // Escritura de correcciones
  update_cachefile(argv[3], corrections); // Actualiza el archivo intermedio

  hashtable_free(corrections);
  hashtable_free(cache);
  trie_destroy(dict);
  return 0;
}

