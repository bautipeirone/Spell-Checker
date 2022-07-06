#ifndef __QUEUE_H__ // Integer Queue
#define __QUEUE_H__

#include "../utils.h"
#include "slist.h"

typedef struct _Queue {
  SList first, last;
} *Queue;

/*
  ** Returns an empty queue
*/
Queue queue_init();

/*
  ** Destroys a queue
*/
void queue_free(Queue q);

/*
  ** Returns if the queue is empty or not
*/
int queue_empty(Queue q);

/*
  ** Returns a copy of the data on the start of the queue
*/
int queue_start(Queue q);

/*
  ** Pushes data to the end of the queue
*/
void queue_push(Queue q, int data);

/*
  ** Removes the start of the queue
*/
void queue_pop(Queue q);

#endif // __QUEUE_H__