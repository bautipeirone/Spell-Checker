#ifndef __SLIST_H__ // Simple linked list
#define __SLIST_H__

typedef struct SNode {
  int data;
  struct SNode *next;
} *SList;

SList slist_init();

int slist_empty(SList head);

SList slist_add_start(SList head, int data);

SList slist_remove_head(SList head);

SList slist_append(SList head, int data);

unsigned slist_len(SList head);

void slist_free(SList head);

void slist_print(SList head);

#endif /* __SLIST_H__ */