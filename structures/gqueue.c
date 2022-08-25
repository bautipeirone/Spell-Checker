#include "gqueue.h"

GQueue gqueue_init() {
  GQueue q = malloc(sizeof(struct _GQueue));
  assert(q != NULL);
  q->last = q->first = NULL;
  return q;
}

void gqueue_free(GQueue q, DestroyFunction destroy) {
  if (q == NULL)
    return;
  glist_free(q->first, destroy);
  free(q);
}

int gqueue_empty(GQueue q) {
  return q == NULL || q->first == NULL;
}

void* gqueue_start(GQueue q, CopyFunction copy) {
  if (gqueue_empty(q))
    return NULL;
  return copy(q->first->data);
}

void gqueue_push(GQueue q, void *data, CopyFunction copy) {
  assert(q != NULL);
  GList new_node = glist_add_start(NULL, data, copy);
  
  if (q->first == NULL)
		q->first = new_node;
	else
		q->last->next = new_node;
  q->last = new_node;
}

void gqueue_pop(GQueue q, DestroyFunction destroy) {
  if (gqueue_empty(q))
    return ;

  q->first = glist_remove_start(q->first, destroy);

  if (q->first == NULL)
    q->last = NULL;
}
