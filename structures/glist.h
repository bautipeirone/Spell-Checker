#ifndef __GLIST_H__
#define __GLIST_H__ // General linked list

#include "../utils.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _GNode {
    void *data;
    struct _GNode *next;
} GNode;

typedef GNode *GList;

/*
** Returns an empty list
*/
GList glist_init();

/*
** Destroys the list
*/
void glist_free(GList list, DestroyFunction destroy);

unsigned glist_len(GList list);

/*
** Returns 1 if the list is empty, 0 otherwise
*/
int glist_empty(GList list);

/*
** Adds an element to the start of the list
*/
GList glist_append_start(GList list, void *data, CopyFunction copy);

/*
** Removes the first element of the list if its not empty, and returns the
** new first node
*/
GList glist_remove_start(GList list, DestroyFunction destroy);

/*
** Visits all nodes on the list, applying the function to the
** data of each one
*/
void glist_visit(GList list, void (*f)(void *data));

/*
** Filters the list with the predicate passed as argument
*/
// GList glist_filter(GList list, CopyFunction c, Predicate p);

/*
** Returns the data from a node that matches with data passed as argument,
** returns NULL if no node matches.
*/
void* glist_search(GList list, void* data, CompareFunction cmp);

#endif /* __GLIST_H__ */
