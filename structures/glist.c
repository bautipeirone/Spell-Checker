#include "glist.h"
#include <stdio.h>

static void gnode_free(GNode node, DestroyFunction destroy) {
  GNode node_to_delete;
  while (node != NULL) {
    node_to_delete = node;
    node = node->next;
    destroy(node_to_delete->data);
    free(node_to_delete);
  }
}

GList glist_init() {
  GList q = malloc(sizeof(struct _GList));
  assert(q != NULL);
  q->last = q->first = NULL;
  return q;
}

void glist_free(GList list, DestroyFunction destroy) {
  if (list == NULL)
    return;
  gnode_free(list->first, destroy);
  free(list);
}

int glist_empty(GList list) {
  return list == NULL || list->first == NULL;
}

unsigned glist_len(GList list) {
  if (list == NULL)
    return 0;
  unsigned len = 1;
  for (GNode node = list->first; node != NULL; len++, node = node->next);
  return len;
}

void glist_add_start(GList list, void *data, CopyFunction copy) {
  assert(list != NULL);
  GNode new_node = malloc(sizeof(struct _GNode));
  assert(new_node != NULL);
  new_node->next = list->first;
  new_node->data = copy(data);

  if (list->last == NULL)
    list->last = new_node;
  list->first = new_node;
}

void glist_add_last(GList list, void *data, CopyFunction copy) {
  assert(list != NULL);
  GNode new_node = malloc(sizeof(struct _GNode));
  assert(new_node != NULL);
  new_node->data = copy(data);
  new_node->next = NULL;
  
  if (list->first == NULL)
		list->first = new_node;
	else
		list->last->next = new_node;
  list->last = new_node;
}

void glist_remove_start(GList list, DestroyFunction destroy) {
  if (glist_empty(list))
    return ;
  GNode tmp = list->first;
  list->first = tmp->next;

  if (list->first == NULL)
    list->last = NULL;

  destroy(tmp->data);
  free(tmp);
}

void glist_visit(GList list, void (*f)(void *data)) {
  if (glist_empty(list))
    return ;
  for (GNode node = list->first; node != NULL; node = node->next)
    f(node->data);
}
