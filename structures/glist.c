#include "glist.h"
#include <stdio.h>

inline GList glist_init() { return NULL; }

inline int glist_empty(GList list) { return list == NULL; }

void glist_free(GList node, DestroyFunction destroy) {
  GList node_to_delete;
  while (!glist_empty(node)) {
    node_to_delete = node;
    node = node->next;
    destroy(node_to_delete->data);
    free(node_to_delete);
  }
}

GList glist_add_start(GList list, void *data, CopyFunction copy) {
  GList new_node = malloc(sizeof(struct _GList));
  assert(new_node != NULL);
  new_node->next = list;
  new_node->data = copy(data);

  return new_node;
}

GList glist_remove_start(GList list, DestroyFunction destroy) {
  if (glist_empty(list))
    return NULL;
  
  GList tmp = list;
  list = tmp->next;

  destroy(tmp->data);
  free(tmp);

  return list;
}
