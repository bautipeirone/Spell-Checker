#include "io.h"
#include "dictionary.h"
#include "checker.h"

int main(int argc, const char* argv[]) {
  //if (argc != 3)
  //  usage();
  (void) argv;
  (void) argc;
  const char* dictionary_path = "diccionario.txt";
  Trie dict = create_dictionary(dictionary_path);
  //(void) dict;
  //HashTable corrections = check_file("input.txt", dict);
  //(void) corrections;
  //write_corrections(argv[2], corrections);

  print_distances(dict, "perro", 5);

  //hashtable_free(corrections);
  trie_destroy(dict);
  return 0;
}
