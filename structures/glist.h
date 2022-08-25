#ifndef __GLIST_H__
#define __GLIST_H__ // General linked lists

#include "../utils.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _GList {
  void *data;
  struct _GList *next;
} *GList;

/*
 * Retorna una lista vacia
*/
GList glist_create();

/*
 * Retorna si la lista esta vacia o no
*/
int glist_empty(GList list);

/*
 * Destruye la lista
*/
void glist_free(GList list, DestroyFunction destroy);

/*
 * Añade un nodo con una copia de dato al inicio de la lista
*/
GList glist_add_start(GList list, void* data, CopyFunction copy);

/*
 * Remueve el primer nodo de la lista, retorna el nuevo comienzo de esta
*/
GList glist_remove_start(GList list, DestroyFunction destroy);


#endif /* __GLIST_H__ */
