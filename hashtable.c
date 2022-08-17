#include "hashtable.h"
#include "glist.h"
#include <assert.h>
#include <stdlib.h>

/* EXTERNAL CHAINING HASH TABLES */

/**
 * Main structure representing a hash table
 */
struct _HashTable {
  GNode *elems;
  unsigned num_elems;
  unsigned size;
  CopyFunction copy;
  CompareFunction cmp;
  DestroyFunction destroy;
  HashFunction hash;
};

HashTable hashtable_init(unsigned size, CopyFunction copy,
                          CompareFunction cmp, DestroyFunction destroy,
                          HashFunction hash) {

  HashTable table = malloc(sizeof(struct _HashTable));
  assert(table != NULL);
  // We ask for 0 initialized memory (every element is a NULL pointer)
  table->elems = calloc(size, sizeof(GNode));
  assert(table->elems != NULL);
  table->num_elems = 0;
  table->size = size;
  table->copy = copy;
  table->cmp = cmp;
  table->destroy = destroy;
  table->hash = hash;

  for (unsigned idx = 0; idx < size; ++idx)
    assert(table->elems[idx] == NULL);

  return table;
}

unsigned hashtable_nelems(HashTable table) { return table->num_elems; }

unsigned hashtable_size(HashTable table) { return table->size; }

static inline float charge_factor(HashTable table) {
  return ((float) table->num_elems / (float) table->size);
}

void hashtable_free(HashTable table) {

  for (unsigned idx = 0; idx < table->size; ++idx)
    gnode_free(table->elems[idx], table->destroy);

  free(table->elems);
  free(table);
  return;
}

void hashtable_insert(HashTable table, void *data) {
  if (charge_factor(table) > CHARGE_FACTOR_THRESH)
    hashtable_resize(table);

  unsigned idx = table->hash(data) % table->size;
  table->elems[idx] = gnode_add_start(table->elems[idx], data, table->copy);
}

void* hashtable_search(HashTable table, void *data) {

  unsigned idx = table->hash(data) % table->size;
  return gnode_search(table->elems[idx], data, table->cmp);
}

void hashtable_remove(HashTable table, void *data) {

  unsigned idx = table->hash(data) % table->size;
  GNode list = table->elems[idx];
  // If list is empty, return
  if (list == NULL)
    return;
  // If the element to remove is in the first node of the list then we update
  // the table to point to the second node and free the first one
  else if (table->cmp(data, list->data) == 0) {
    table->num_elems--;
    table->elems[idx] = gnode_remove_start(list, table->destroy);
    return;
  } else {
    while (list->next != NULL) {
      if (table->cmp(data, list->next->data) == 0) {
        GNode tmp = list->next;
        table->num_elems--;
        list->next = tmp->next;
        table->destroy(tmp->data);
        free(tmp);
        break;
      }
    }
  }
}

void hashtable_resize(HashTable table) {
  GNode *tmp_elems = table->elems;
  unsigned tmp_size = table->size;
  table->num_elems = 0;
  table->size *= 2;
  table->elems = calloc(table->size, sizeof(GNode));
  assert(table->elems != NULL);
  
  // Iterate over every node of every list on the table
  // adding each element to the resized array
  for (unsigned i = 0; i < tmp_size; ++i) {
    GNode node = tmp_elems[i], tmp;
    while (node != NULL) {
      hashtable_insert(table, node->data);
      tmp = node->next;
      table->destroy(node->data);
      free(node);
      node = tmp;
    }
  }
  free(tmp_elems);
  return;
}

void hashtable_iterate(HashTable table, VisitFunctionExtra visit, void *extra) {
  for (unsigned i = 0; i < table->size; i++)
    if (table->elems[i] != NULL)
      gnode_visit(visit, table->elems[i], extra);
}
