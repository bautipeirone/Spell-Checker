#include "bheap.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static inline unsigned parent_pos(const unsigned pos) {
  if (!pos)
    return 0;
  return (pos - 1) / 2;
}

static inline unsigned lchild_pos(const unsigned pos) { return pos * 2 + 1; }

static inline unsigned rchild_pos(const unsigned pos) { return (pos + 1) * 2; }

BHeap bheap_init(unsigned size, CompareFunction cmp, CopyFunction copy,
  DestroyFunction destroy) {
  BHeap heap = malloc(sizeof(struct _BHeap));
  assert(heap != NULL);
  heap->arr = malloc(sizeof(void*) * size);
  assert(heap->arr != NULL);
  heap->size = size;
  heap->last = 0;
  heap->cmp = cmp;
  heap->copy = copy;
  heap->destroy = destroy;
  return heap;
}

void bheap_destroy(BHeap heap) {
  if (heap == NULL)
    return;
  for (unsigned i = 0; i < heap->last; i++)
    heap->destroy(heap->arr[i]);
  free(heap->arr);
  free(heap);
}

int bheap_empty(BHeap heap) {
  return heap == NULL || heap->last == 0;
}

void bheap_iter(BHeap heap, VisitFunction visit) {
  for (unsigned i = 0; i < heap->last; ++i)
    visit(heap->arr[i]);
}

void bheap_insert(BHeap heap, void* data) {
  if (heap->last == heap->size)
    bheap_resize(heap);
  heap->arr[heap->last] = heap->copy(data);
  bheap_float_element(heap, heap->last++);
}

void bheap_remove(BHeap heap, void* data) {
  int stop = 0;
  unsigned i;
  for (i = 0; i < heap->last && !stop; ++i)
    if (heap->cmp(data, heap->arr[i]) == 0)
      stop = 1;
  // Data is not in the heap
  if (stop == 0)
    return;
  // Swap the element to be removed with the last one
  heap->destroy(heap->arr[i - 1]);
  heap->arr[i - 1] = heap->arr[--heap->last];
  bheap_sink_element(heap, i - 1);
}

void bheap_float_element(BHeap heap, unsigned pos) {
  assert(pos < heap->size);
  void* element = heap->arr[pos];
  unsigned parent = parent_pos(pos);
  while (pos > 0 && heap->cmp(element, heap->arr[parent]) > 0) {
    heap->arr[pos] = heap->arr[parent];
    pos = parent;
    parent = parent_pos(pos);
  }
  heap->arr[pos] = element;
}

void bheap_sink_element(BHeap heap, unsigned pos) {
  assert(pos < heap->size);
  unsigned lc, rc, max_child; // Posiciones de hijo izquierdo y derecho
  void *element = heap->arr[pos];
  do {
    lc = lchild_pos(pos);
    rc = rchild_pos(pos);
    if (lc >= heap->last)
      break;
    else if (rc >= heap->last)
      max_child = lc;
    else
      max_child = heap->cmp(heap->arr[lc], heap->arr[rc]) <= 0 ? rc : lc;
    if (heap->cmp(heap->arr[pos], heap->arr[max_child]) < 0) {
      heap->arr[pos] = heap->arr[max_child];
      pos = max_child;
    } else
      break;
  } while (1);
  heap->arr[pos] = element;
}

void bheap_resize(BHeap heap) {
  heap->size *= 2;
  heap->arr = realloc(heap->arr, sizeof(void*) * heap->size);
  assert(heap->arr != NULL);
}

BHeap bheap_from_array(void** arr, unsigned len, CompareFunction cmp,
  CopyFunction copy, DestroyFunction destroy) {

  BHeap heap = bheap_init(len, cmp, copy, destroy);
  memcpy(heap->arr, arr, sizeof(void*) * len);
  heap->last = len;

  for (int i = heap->last - 1; i >= 0; --i)
    bheap_sink_element(heap, i);
  return heap;
}

void* bheap_remove_max(BHeap heap) {
  if (bheap_empty(heap))
    return NULL;
  void *element = heap->arr[0];
  heap->arr[0] = heap->arr[--heap->last];
  bheap_sink_element(heap, 0);
  return element;
}
