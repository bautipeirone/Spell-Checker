#include "hashtable.h"
#include <assert.h>
#include <stdlib.h>

/*  OPEN ADDRESSING HASH TABLES  */
/* LINEAR PROBING IMPLEMENTATION */

/**
 * Cells in which the hash table data will be stored in
 */
typedef struct {
  void *data;
} HashBox;

/*
 * Pointer to a special location to indicate REMOVED elements
*/
static void* REMOVED;

/**
 * Main structure for the hash table
 */
struct _HashTable {
  HashBox *elems;
  unsigned num_elems;
  unsigned size;
  CopyFunction copy;
  CompareFunction cmp;
  DestroyFunction destroy;
  HashFunction hash;
};

/**
 * Creates a new hash table, with the size and functions given
 */
HashTable hashtable_init(unsigned size, CopyFunction copy,
                          CompareFunction cmp, DestroyFunction destroy,
                          HashFunction hash) {

  HashTable table = malloc(sizeof(struct _HashTable));
  assert(table != NULL);
  table->elems = malloc(sizeof(HashBox) * size);
  assert(table->elems != NULL);
  table->num_elems = 0;
  table->size = size;
  table->copy = copy;
  table->cmp = cmp;
  table->destroy = destroy;
  table->hash = hash;

  for (unsigned idx = 0; idx < size; ++idx)
    table->elems[idx].data = NULL;

  return table;
}

/**
 * Returns the number of elements in the table
 */
unsigned hashtable_nelems(HashTable table) { return table->num_elems; }

/**
 * Returns the size of the table
 */
unsigned hashtable_size(HashTable table) { return table->size; }

static inline float charge_factor(HashTable table) {
  return ((float) table->num_elems / (float) table->size);
}

/**
 * Destroys the table
 */
void hashtable_free(HashTable table) {

  // Destroy each element on the table
  for (unsigned idx = 0; idx < table->size; ++idx)
    if (table->elems[idx].data != NULL)
      table->destroy(table->elems[idx].data);

  // Free the boxes and the table
  free(table->elems);
  free(table);
  return;
}

/**
 * Inserta un data en la table, o lo reemplaza si ya se encontraba.
 */
void hashtable_insert(HashTable table, void *data) {

  // If charge factor is greater than a certain threshold, resize the table
  if (charge_factor(table) > CHARGE_FACTOR_THRESH)
    hashtable_resize(table);

  // Calculate the index of data in the table
  unsigned idx = table->hash(data) % table->size;

  int inserted = 0;
  // Search for an empty position checking one by one
  for (unsigned i = 0; i < table->size && !inserted; ++i) {
    // Insert data if empty or if previous data in the cell was REMOVED
    if (table->elems[idx].data == NULL || table->elems[idx].data == REMOVED) {
      table->num_elems++;
      table->elems[idx].data = table->copy(data);
      inserted = 1;
    // Overwrite data if already in the table
    } else if (table->cmp(table->elems[idx].data, data) == 0) {
      table->destroy(table->elems[idx].data);
      table->elems[idx].data = table->copy(data);
      inserted = 1;
    }
    idx = (idx + 1) % table->size;
  }
  return;
}

void* hashtable_search(HashTable table, void *data) {

  unsigned idx = table->hash(data) % table->size;
  int stop = 0;

  // Stop if NULL is found or every element is checked
  for (unsigned i = 0; !stop && i < table->size; ++i) {
    if (table->elems[idx].data == NULL)
      stop = 1;
    else if (table->cmp(table->elems[idx].data, data) == 0)
      return table->elems[idx].data;
    idx = (idx + 1) % table->size;
  }
  return NULL;
}

void hashtable_remove(HashTable table, void *data) {

  unsigned idx = table->hash(data) % table->size;

  for (unsigned i = 0; i < table->size; ++i) {
    if (table->elems[idx].data == NULL)
      return;
    // Vaciar la casilla si hay coincidencia.
    else if (table->cmp(table->elems[idx].data, data) == 0) {
      table->num_elems--;
      table->destroy(table->elems[idx].data);
      // Assign to the cell the REMOVED symbolic value. The reason why NULL is
      // not used is because NULL indicates the end of search.
      table->elems[idx].data = REMOVED;
      return;
    }
    idx = (idx + 1) % table->size;
  }
}

void hashtable_resize(HashTable table) {
  HashBox *tmp_elems = table->elems;
  unsigned tmp_size = table->size;
  table->size *= 2;
  table->num_elems = 0;
  table->elems = malloc(sizeof(HashBox) * table->size);
  assert(table->elems != NULL);
  
  for (unsigned i = 0; i < tmp_size; ++i) {
    if (tmp_elems[i].data == NULL || tmp_elems[i].data == REMOVED)
      continue;
    hashtable_insert(table, tmp_elems[i].data);
    table->destroy(tmp_elems[i].data);
  }

  free(tmp_elems);
}
