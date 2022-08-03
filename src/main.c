#include "io.h"
#include "dictionary.h"
#include "checker.h"
#include "../structures/dtree.h"
#include "distances.h"

int main(int argc, const char* argv[]) {
  if (argc != 3)
    usage(); 
  
  const char* dictionary_path = "diccionario.txt";
  DTree dist_dict = dtree_init((DistanceFunction) edit_distance);
  HashTable dict = create_dictionary(dictionary_path, dist_dict);
  HashTable corrections = check_file(argv[1], dict, dist_dict);
  write_corrections(argv[2], corrections);
  
  dtree_free(dist_dict);
  hashtable_free(corrections);
  hashtable_free(dict);
  return 0;
}
