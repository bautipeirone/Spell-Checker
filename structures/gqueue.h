#ifndef _GQUEUE_H_

#define _GQUEUE_H_
#include "../utils.h"
#include "glist.h"

typedef struct _GQueue {
  GList first, last;
} *GQueue;

/*
 * Retorna una cola vacia 
*/
GQueue gqueue_init();

/*
 * Destruye una cola
*/
void gqueue_free(GQueue q, DestroyFunction destroy);

/*
 * Retorna 1 si es cola esta vacia, 0 en caso contrario
*/
int gqueue_empty(GQueue q);

/*
 * Retorna una copia del dato al comienzo de la cola, NULL si la cola esta vacia
*/
void* gqueue_start(GQueue q, CopyFunction copy);

/*
 * Inserta una copia del dato al final de la cola
*/
void gqueue_push(GQueue q, void *data, CopyFunction copy);

/*
 * Remueve el elemento al comienzo de la cola
*/
void gqueue_pop(GQueue q, DestroyFunction destroy);

#endif // _GQUEUE_H_