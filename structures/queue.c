#include "queue.h"
#include <stdlib.h>
#include <assert.h>

Queue queue_init() {
  Queue q = malloc(sizeof(struct _Queue));
  assert(q != NULL);
  q->last = q->first = NULL;
  return q;
}

void queue_free(Queue q) {
  slist_free(q->first);
  free(q);
}

int queue_empty(Queue q) { return q->first == NULL; }

int queue_start(Queue q) {
  if (queue_empty(q))
    assert(0);
  return q->first->data;
}

void queue_push(Queue q, int data) {
  SList new_node = slist_add_start(NULL, data);
  // Si la cola es vacia, se actualiza el primer elemento
  if (queue_empty(q))
      q->first = new_node;
  // Sino, entonces hay un ultimo elemento,
  // por lo que el nuevo nodo debe ubicarse luego de este
  else
    q->last->next = new_node;
  q->last = new_node;
}

void queue_pop(Queue q) {
    if (queue_empty(q)) return ;
    SList to_remove = q->first;
    
    q->first = q->first->next;
    // Si ya no hay elementos, el ultimo ahora no apunta a ningun nodo
    if (q->first == NULL)
        q->last = NULL;
    
    free(to_remove);
}

