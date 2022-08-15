#include "gqueue.h"

GQueue gqueue_init() {
  return glist_init();
}

void gqueue_free(GQueue q, DestroyFunction destroy) {
  glist_free(q, destroy);
}

int gqueue_empty(GQueue q) { return glist_empty(q); }

void* gqueue_start(GQueue q, CopyFunction copy) {
	if (gqueue_empty(q))
		return NULL;
	return copy(q->first->data);
}

void gqueue_push(GQueue q, void *data, CopyFunction copy) {
  glist_add_last(q, data, copy);
}

void gqueue_pop(GQueue q, DestroyFunction destroy) {
  glist_remove_start(q, destroy);
}
