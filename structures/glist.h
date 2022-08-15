#ifndef __GLIST_H__
#define __GLIST_H__ // General linked list

#include "../utils.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _GNode {
  void *data;
  struct _GNode *next;
} *GNode;

typedef struct _GList {
  GNode first, last;
} *GList;

/*
** Retorna una lista vacia
*/
GList glist_init();

/*
** Destruye la lista
*/
void glist_free(GList list, DestroyFunction destroy);

/*
** Retorna la longitud de la lista
*/
unsigned glist_len(GList list);

/*
** Retorna 1 si la lista es vacia, 0 en caso contrario
*/
int glist_empty(GList list);

/*
** Añade un elemento al comienzo de la lista
*/
void glist_add_start(GList list, void *data, CopyFunction copy);

/*
** Añade un nodo al final de la lista
*/
void glist_add_last(GList list, void *data, CopyFunction copy);

/*
** Remueve el primer elemento de la lista si esta no es vacia
*/
void glist_remove_start(GList list, DestroyFunction destroy);

/*
** Visita todos los nodos de la lista, aplicando la funcion en el
** dato de cada nodo
*/
void glist_visit(GList list, void (*f)(void *data));

#endif /* __GLIST_H__ */
