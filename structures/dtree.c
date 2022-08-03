#include "dtree.h"
#include "../src/distances.h"
#include <stdlib.h>
#include <assert.h>

static inline int dtree_node_cmp(DTree_Node node1, DTree_Node node2) {
  return node1->dist_to_parent - node2->dist_to_parent;
}

static inline unsigned dtree_node_hash(DTree_Node node) {
  return node->dist_to_parent;
}

void dtree_node_free(DTree_Node root) {
  hashtable_free(root->children);
  free(root->word);
  free(root);
}

DTree dtree_init(DistanceFunction distance) {
  DTree tree = malloc(sizeof(struct _DTree));
  assert(tree != NULL);
  tree->root = NULL;
  tree->distance = distance;
  return tree;
}

DTree_Node dtree_node_init(char *word, unsigned dist_parent) {
  DTree_Node node = malloc(sizeof(struct _DTree_Node));
  assert(node != NULL);
  node->word = copy_str(word);
  node->len = strlen(word);
  node->dist_to_parent = dist_parent;
  node->children = hashtable_init(7, id, (CompareFunction) dtree_node_cmp,
            (DestroyFunction) dtree_node_free, (HashFunction) dtree_node_hash);
  return node;
}

void dtree_node_insert(DTree_Node root, char* word, unsigned len,
                      DistanceFunction dist) {
  unsigned d = dist(root->word, word, root->len, len);
  if (d == 0) // No insertamos repetidos
    return ;
  struct _DTree_Node buf;
  buf.dist_to_parent = d;
  DTree_Node child = hashtable_search(root->children, &buf);
  if (child == NULL)
    hashtable_insert(root->children, dtree_node_init(word, d));
  else {
    dtree_node_insert(child, word, len, dist);
  }
}

void dtree_insert(DTree tree, char* word) {
  if (tree->root == NULL)
    // Definimos la distancia de la raiz a su padre como 0
    tree->root = dtree_node_init(word, 0);
  else
    dtree_node_insert(tree->root, word, strlen(word), tree->distance);
}

void dtree_free(DTree tree) {
  dtree_node_free(tree->root);
  free(tree);
}

void dtree_node_search(DTree_Node root, char *word, unsigned len,
              unsigned max_dist, DistanceFunction dist, BHeap heap) {
  unsigned d = dist(word, root->word, len, root->len);
  if (d <= MAX_SEARCH_DISTANCE) {
    WordDistance wd = init_wd(root->word, d);
    bheap_insert(heap, wd);
  }
  struct _DTree_Node buf, *child;
  for (int i = d - max_dist; i <= (int)(d + max_dist); ++i) {
    if (i <= 0)
      continue;
    buf.dist_to_parent = i;
    child = hashtable_search(root->children, (void*) &buf);
    if (child != NULL)
      dtree_node_search(child, word, len, max_dist, dist, heap);
  }
}

BHeap dtree_search_by_distance(DTree tree, char *word, unsigned max_dist) {
  if (tree == NULL || tree->root == NULL)
    return NULL;
  BHeap heap = bheap_init(100, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  dtree_node_search(tree->root, word, strlen(word), max_dist, tree->distance, heap);
  return heap;
}
