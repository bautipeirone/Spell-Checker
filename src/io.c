#include "io.h"

int read_word(FILE* fp, char buf[MAX_LEN_WORD + 1], unsigned *line_number) {
  int c, i = 0, stop = 0;
  do {
    c = fgetc(fp);
    if (c == '\n')
      ++(*line_number);
  } while (c != EOF && !isalpha(c));

  if (c == EOF)
    return 0;

  ungetc(c, fp);
  
  while (i < MAX_LEN_WORD && !stop) {
    c = getc(fp);
    if (c == EOF) {
      buf[i] = '\0';
      return 0;
    } else if (isalpha(c))
      buf[i++] = c;
    else
      stop = 1;
  }
  if (c == '\n')
    ++(*line_number);

  buf[i] = '\0';

  return 1;
}

void write_correction(WrongWord w, FILE *fp) {
  fputs("Lineas ", fp);
  while (!queue_empty(w->lines)) {
    fprintf(fp, "%d, ", queue_start(w->lines));
    queue_pop(w->lines);
  }
  fprintf(fp, "\"%s\" no esta en el diccionario\n", w->word);
  if (w->num == 0)
    fputs("No se han encontrado sugerencias para la palabra", fp);
  else {
    fputs("Quizas quiso decir: ", fp);
    for (int i = 0; i < w->num; ++i)
      fprintf(fp, "%s, ", w->suggests[i]);
  }
  fputc('\n', fp);
  fputc('\n', fp);
  free_wrongword(w);
}

void write_corrections(const char *path, HashTable corrections) {
  FILE *fp = fopen(path, "w");
  assert(fp != NULL);
  hashtable_iterate(corrections, (VisitFunctionExtra) write_correction, fp);
  fclose(fp);
}

void usage() {
  puts("Uso del programa:");
  puts("./main <archivoEntrada> <archivoSalida>");
  exit(EXIT_FAILURE);
}
