#include "utils.h"

void *id(void *p) { return p; }

void null(void *p) { (void) p; return; }

unsigned KRHash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}

char* copy_str(const char *str) {
  char *copy = malloc(strlen(str) + 1);
  assert(copy != NULL);
  strcpy(copy, str);
  return copy;
}
