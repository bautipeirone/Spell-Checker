#ifndef __HEAP_H__
#define __HEAP_H__ // Max Binary Heaps

#include "../utils.h"

typedef struct _BHeap {
    void **arr;
    unsigned size;
    unsigned last;
    CompareFunction cmp;
    CopyFunction copy;
    DestroyFunction destroy;
} *BHeap;

/*
 * Inicializa un heap con el tamaño y las funciones dadas
*/
BHeap bheap_init(size_t size, CompareFunction cmp, CopyFunction copy,
                DestroyFunction destroy);

/*
 * Destruye un heap
*/
void bheap_destroy(BHeap heap);

/*
 * Evalua si el heap esta vacio o no
*/
int bheap_empty(BHeap heap);

/*
 * Itera sobre los elementos del heap aplicando la funcion
*/
void bheap_iter(BHeap heap, VisitFunction visit);

/*
 * Inserta un dato en el heap en su posicion
*/
void bheap_insert(BHeap heap, void *data);

/*
 * Remueve el dato del heap si este se encuentra, preservando la estructura
*/
void bheap_remove(BHeap heap, void *data);

/*
 * Redimensiona el heap
*/
void bheap_resize(BHeap heap);

/*
 * Retorna una copia del dato con mayor prioridad
*/
void* bheap_get_max(BHeap heap);

/*
 * Remueve el dato mayor del heap, preservando su estructura
*/
void bheap_remove_max(BHeap heap);

/*
 * Flota el elemento en la posicion dada hasta su ubicacion correcta
*/
void bheap_float_element(BHeap heap, unsigned pos);

/*
 * Hunde el elemento en la posicion dada hasta su ubicacion correcta
*/
void bheap_sink_element(BHeap heap, unsigned pos);

#endif /* __HEAP_H__ */
