#ifndef __DTREE_H__
#define __DTREE_H__ // Distance Trees

#include "../utils.h"
#include "hashtable.h"
#include "bheap.h"

typedef struct _DTree_Node {
  unsigned dist_to_parent;
  unsigned len;
  char* word;
  HashTable children;
} *DTree_Node;

typedef struct _DTree {
  DTree_Node root;
  DistanceFunction distance;
} *DTree;

DTree dtree_init(DistanceFunction distance);

void dtree_insert(DTree root, char* word);

BHeap dtree_search_by_distance(DTree root, char* word, unsigned dist);

void dtree_free(DTree root);

#endif /* __DTREE_H__ */
