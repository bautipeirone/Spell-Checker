#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "../utils.h"
#define CHARGE_FACTOR_THRESH 0.7

typedef struct _HashTable *HashTable;

/**
 * Crea una tabla hash con el tamaño y las funciones dadas
*/
HashTable hashtable_init(unsigned size, CopyFunction copy,
                          CompareFunction cmp, DestroyFunction destroy,
                          HashFunction hash);

/**
 * Retorna la cantidad de elementos en la tabla
*/
unsigned hashtable_nelems(HashTable table);

/**
 * Retorna el tamaño de la tabla
*/
unsigned hashtable_size(HashTable table);

/*
 * Retorna el arreglo de datos de la tabla
*/
void** hashtable_elems(HashTable table);

/**
 * Destruye la tabla
*/
void hashtable_free(HashTable table);

/**
 * Inserta una copia del dato en la tabla, lo sobrescribe si ya estaba
 */
void hashtable_insert(HashTable table, void *data);

/**
 * Retorna el elemento que coincida con el dato, o NULL si ninguno lo hace
 */
void* hashtable_search(HashTable table, void *data);

/**
 * Remueve el dato de la tabla
 */
void hashtable_remove(HashTable table, void *data);

/*
 * Duplica el tamaño de la tabla, rehashea los elementos a sus nuevas posiciones
*/
void hashtable_resize(HashTable table);

/*
 * Aplica la funcion a todas las casillas no vacias o eliminadas
*/
void hashtable_iterate(HashTable table, VisitFunctionExtra visit, void *extra);

#endif /* __HASHTABLE_H__ */