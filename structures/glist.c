#include "glist.h"
#include <stdio.h>

GList glist_init() { return NULL; }

void glist_free(GList list, DestroyFunction destroy) {
    GNode *node_to_delete;
    while (list != NULL) {
        node_to_delete = list;
        list = list->next;
        destroy(node_to_delete->data);
        free(node_to_delete);
    }
}

int glist_empty(GList list) { return (list == NULL); }

unsigned glist_len(GList list) {
  if (list == NULL)
    return 0;
  unsigned len;
  for (len = 1; list->next != NULL; len++, list = list->next);
  return len;
}

GList glist_append_start(GList list, void *data, CopyFunction copy) {
    GNode *new_node = malloc(sizeof(GNode));
    assert(new_node != NULL);
    new_node->next = list;
    new_node->data = copy(data);
    return new_node;
}

GList glist_remove_start(GList list, DestroyFunction destroy) {
    GNode *tmp;
    if (glist_empty(list))
        return NULL;
    tmp = list;
    list = list->next;
    destroy(tmp->data);
    free(tmp);
    return list;
}

void glist_visit(GList list, void (*f)(void *data)) {
    for (GNode *node = list; node != NULL; node = node->next)
        f(node->data);
}

void* glist_search(GList list, void* data, CompareFunction cmp) {
    for (GNode *node = list; node != NULL; node = node->next)
        if (cmp(data, node->data) == 0)
            return node->data;
    return NULL;
}
