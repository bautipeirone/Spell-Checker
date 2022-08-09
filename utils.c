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

int min(const int x, const int y) {
  if (x > y)
    return y;
  return x;
}

int min3(int x, int y, int z) {
  return min(min(x, y), z);
}

int min4(const int x, const int y, const int z, const int w) {
  return min(min(x, y), min(z,w));
}

void lower_str(char *str) {
  for (; *str != '\0'; str++)
    *str = tolower(*str);
}
