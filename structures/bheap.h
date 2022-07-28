#ifndef __HEAP_H__
#define __HEAP_H__ // Max Binary Heaps

#include "../../utils.h"

typedef struct _BHeap {
    void **arr;
    unsigned size;
    unsigned last;
    CompareFunction cmp;
    CopyFunction copy;
    DestroyFunction destroy;
} *BHeap;

/*
 * Initializes a heap with a size given and the provided compare, copy
 * and destroy function
*/
BHeap bheap_init(unsigned size, CompareFunction cmp, CopyFunction copy,
                DestroyFunction destroy);

/*
 * Destroy the heap
*/
void bheap_destroy(BHeap heap);

/*
 * Returns 1 if heap is empty, 0 otherwise
*/
int bheap_empty(BHeap heap);

/*
 * Iterate over all the elements on the heap applying the visit function
*/
void bheap_iter(BHeap heap, VisitFunction visit);

/*
 * Inserts data in the heap, preserving its structure
*/
void bheap_insert(BHeap heap, void *data);

/*
 * Removes data from the heap if found, preserving the
 * structure of a binary heap
*/
void bheap_remove(BHeap heap, void *data);

/*
 * Resizes the heap to the double of its capacity
*/
void bheap_resize(BHeap heap);

/*
 * Remove and return the max element of the heap while
 * preserving the structure
*/
void* bheap_remove_max(BHeap heap);

/*
 * Creates a binary heap from an array
*/
BHeap bheap_from_array(void **arr, unsigned len, CompareFunction cmp,
                        CopyFunction copy, DestroyFunction destroy);

/*
 * Sifts an element up until the condition of a binary heap is satisfied
*/
void bheap_float_element(BHeap heap, unsigned pos);

/*
 * Sifts an element down until the condition of a binary heap is satisfied
*/
void bheap_sink_element(BHeap heap, unsigned pos);

#endif /* __HEAP_H__ */