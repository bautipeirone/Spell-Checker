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

GNode gnode_add_start(GNode, void*, CopyFunction);

GNode gnode_remove_start(GNode head, DestroyFunction destroy);

void* gnode_search(GNode, void*, CompareFunction);

/*
** Visita todos los nodos de la lista, aplicando la funcion en el
** dato de cada nodo
*/
void gnode_visit(VisitFunctionExtra visit, GNode head, void* extra);

void gnode_free(GNode head, DestroyFunction destroy);

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

#endif /* __GLIST_H__ */
