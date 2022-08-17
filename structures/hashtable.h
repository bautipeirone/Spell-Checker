#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "../utils.h"
#define CHARGE_FACTOR_THRESH 0.7F

typedef struct _HashTable *HashTable;

/**
 * Initializes a hash table with a size given, and the required
 * functions used in the table
 */
HashTable hashtable_init(unsigned size, CopyFunction copy,
                          CompareFunction cmp, DestroyFunction destroy,
                          HashFunction hash);

/**
 * Returns the number of elements in the table
 */
unsigned hashtable_nelems(HashTable table);

/**
 * Returns the size of the table
 */
unsigned hashtable_size(HashTable table);

/*
 * FIXME: Try to avoid this
*/
void** hashtable_elems(HashTable table);

/**
 * Destroys the table
 */
void hashtable_free(HashTable table);

/**
 * Inserts data in the table, or replaces it if already in the table
 */
void hashtable_insert(HashTable table, void *data);

/**
 * Returns the element that matches with data, or NULL if not in the table
 */
void* hashtable_search(HashTable table, void *data);

/**
 * Removes the element of the table that matches with data
 */
void hashtable_remove(HashTable table, void *data);

/*
 * Resize the table to the double of its size, rehashing all elements
 * in the new table
*/
void hashtable_resize(HashTable table);

/*
 * Applies visit to every non NULL element on the table
*/
void hashtable_iterate(HashTable table, VisitFunctionExtra visit, void *extra);

#endif /* __HASHTABLE_H__ */