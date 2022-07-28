#include "utils.h"
// #include "structures/glist.h"
// #include "structures/gqueue.h"
#include "src/dictionary.h"

int main() {
  Trie dictionary = create_dictionary("diccionario.txt");
  FILE *fp = fopen("output1.txt", "w");
  trie_iter(dictionary, (VisitFunctionExtra) fputs, fp);
  fclose(fp);
  trie_destroy(dictionary);
  return 0;
}