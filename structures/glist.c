#include "glist.h"
#include <stdio.h>

void gnode_free(GNode node, DestroyFunction destroy) {
  GNode node_to_delete;
  while (node != NULL) {
    node_to_delete = node;
    node = node->next;
    destroy(node_to_delete->data);
    free(node_to_delete);
  }
}

GNode gnode_add_start(GNode head, void *data, CopyFunction copy) {
  GNode new_node = malloc(sizeof(struct _GNode));
  assert(new_node != NULL);
  new_node->next = head;
  new_node->data = copy(data);

  return new_node;
}

GNode gnode_remove_start(GNode head, DestroyFunction destroy) {
  if (head == NULL)
    return NULL;
  
  GNode tmp = head;
  head = tmp->next;

  destroy(tmp->data);
  free(tmp);

  return head;
}

void* gnode_search(GNode head, void* data, CompareFunction cmp) {
    for (GNode node = head; node != NULL; node = node->next)
        if (cmp(data, node->data) == 0)
            return node->data;
    return NULL;
}

void gnode_visit(VisitFunctionExtra f, GNode head, void* extra) {
  if (head == NULL)
    return ;
  for (GNode node = head; node != NULL; node = node->next)
    f(node->data, extra);
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
  list->first = gnode_add_start(list->first, data, copy);
  if (list->last == NULL)
    list->last = list->first;
}

void glist_add_last(GList list, void *data, CopyFunction copy) {
  assert(list != NULL);
  GNode new_node = gnode_add_start(NULL, data, copy);
  
  if (list->first == NULL)
		list->first = new_node;
	else
		list->last->next = new_node;
  list->last = new_node;
}

void glist_remove_start(GList list, DestroyFunction destroy) {
  if (glist_empty(list))
    return ;

  list->first = gnode_remove_start(list->first, destroy);

  if (list->first == NULL)
    list->last = NULL;
}
