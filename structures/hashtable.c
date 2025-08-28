#include "hashtable.h"
#include <assert.h>
#include <stdlib.h>

/*  TABLA HASH - DIRECCIONAMIENTO ABIERTO  */
/*            LINEAR PROBING               */

/*
 * Puntero que representa a las casillas eliminadas
*/
static void* REMOVED;

/**
 * Estructura de la Tabla Hash
 */
struct _HashTable {
  void **elems;           // Arreglo de datos
  unsigned num_elems;     // Numero de casillas ocupadas / Elementos en la tabla
  unsigned size;          // Tamaño de la tabla
  CopyFunction copy;      // Funcion de copia de datos
  CompareFunction cmp;    // Funcion de comparacion de datos
  DestroyFunction destroy;// Funcion de destruccion de los datos en la tabla
  HashFunction hash;      // Funcion de hashing para los datos a insertar
};

HashTable hashtable_init(unsigned size, CopyFunction copy,
            CompareFunction cmp, DestroyFunction destroy, HashFunction hash) {

  HashTable table = malloc(sizeof(struct _HashTable));
  assert(table != NULL);
  table->elems = malloc(sizeof(void*) * size);
  assert(table->elems != NULL);
  table->num_elems = 0;
  table->size = size;
  table->copy = copy;
  table->cmp = cmp;
  table->destroy = destroy;
  table->hash = hash;

  for (unsigned idx = 0; idx < size; ++idx)
    table->elems[idx] = NULL;

  return table;
}

inline unsigned hashtable_nelems(HashTable table) { return table->num_elems; }

inline unsigned hashtable_size(HashTable table) { return table->size; }

inline void** hashtable_elems(HashTable table) { return table->elems; }

static inline double charge_factor(HashTable table) {
  return ((double) table->num_elems / (double) table->size);
}

void hashtable_free(HashTable table) {
  for (unsigned idx = 0; idx < table->size; ++idx)
    if (table->elems[idx] != NULL)
      table->destroy(table->elems[idx]);

  free(table->elems);
  free(table);
  return;
}

void hashtable_insert(HashTable table, void *data) {

  // Si se supera un cierto limite, se hace un redimensionado de la tabla
  if (charge_factor(table) > CHARGE_FACTOR_THRESH)
    hashtable_resize(table);

  unsigned idx = table->hash(data) % table->size;
  int inserted = 0;
  // Busqueda uno por uno en la tabla, hasta insertar el elemento
  for (unsigned i = 0; i < table->size && !inserted; ++i) {
    // Si la celda esta vacia o marcada como removida se inserta
    if (table->elems[idx] == NULL || table->elems[idx] == REMOVED) {
      table->num_elems++;
      table->elems[idx] = table->copy(data);
      inserted = 1;
    // Sobrescribir el dato si ya se encontraba
    } else if (table->cmp(table->elems[idx], data) == 0) {
      table->destroy(table->elems[idx]);
      table->elems[idx] = table->copy(data);
      inserted = 1;
    }
    idx = (idx + 1) % table->size;
  }
  return ;
}

void* hashtable_search(HashTable table, void *data) {

  unsigned idx = table->hash(data) % table->size;
  int stop = 0;

  // Detenerse si fue encontrado o se dio una vuelta entera
  for (unsigned i = 0; !stop && i < table->size; ++i) {
    if (table->elems[idx] == NULL)
      stop = 1;
    else if (table->elems[idx] != REMOVED
          && table->cmp(table->elems[idx], data) == 0)
      return table->elems[idx];
    idx = (idx + 1) % table->size;
  }
  return NULL;
}

void hashtable_remove(HashTable table, void *data) {

  unsigned idx = table->hash(data) % table->size;

  for (unsigned i = 0; i < table->size; ++i) {
    if (table->elems[idx] == NULL)
      return;
    // Vaciar la casilla si hay coincidencia.
    else if (table->elems[idx] != REMOVED
          && table->cmp(table->elems[idx], data) == 0) {
      table->num_elems--;
      table->destroy(table->elems[idx]);
      // Marcar la casilla con el puntero REMOVED
      table->elems[idx] = REMOVED;
      return;
    }
    idx = (idx + 1) % table->size;
  }
}

void hashtable_resize(HashTable table) {
  void **tmp_elems = table->elems;
  unsigned tmp_size = table->size;
  table->size *= 2; // Se duplica el tamaño de la tabla
  table->num_elems = 0;
  table->elems = malloc(table->size * sizeof(void*));
  assert(table->elems != NULL);
  // Inicializar la tabla a 0
  for (unsigned i = 0; i < table->size; table->elems[i++] = NULL);

  CopyFunction copy = table->copy;
  DestroyFunction destroy = table->destroy;
  // Se reemplazan las funciones de copia y destruccion para evitar copias
  // innecesarias al mover un elemento a la nueva tabla
  table->copy = id;
  table->destroy = null;

  for (unsigned i = 0; i < tmp_size; ++i) {
    if (tmp_elems[i] == NULL || tmp_elems[i] == REMOVED)
      continue;
    hashtable_insert(table, tmp_elems[i]);
  }

  table->copy = copy;
  table->destroy = destroy;

  free(tmp_elems);
}

void hashtable_iterate(HashTable table, VisitFunctionExtra visit, void *extra) {
  // Recorrer la tabla y aplicar la funcion a las casillas no vacias
  for (unsigned i = 0; i < table->size; i++)
    if (table->elems[i] != NULL && table->elems[i] != REMOVED)
      visit(table->elems[i], extra);
}
