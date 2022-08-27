#include "utils.h"

inline void *id(void *p) { return p; }

inline void null(void *p) { (void) p; return; }

unsigned KRHash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}

int* copy_int(int *p) {
  int* copy = malloc(sizeof(int));
  assert(copy != NULL);
  *copy = *p;
  return copy;
}

char* copy_str(const char *s) {
  int len = strlen(s);
  char* copy = malloc(len + 1);
  memcpy(copy, s, len + 1);

  return copy;
}

void lower_str(char *str) {
  for (; *str != '\0'; str++)
    *str = tolower(*str);
}
