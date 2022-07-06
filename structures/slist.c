#include "slist.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

SList slist_init() { return NULL; }

int slist_empty(SList head) { return head == NULL; }

SList slist_add_start(SList head, int data) {
    SList new_node = malloc(sizeof(struct SNode));
    assert(new_node != NULL);
    new_node->data = data;
    new_node->next = head;
    return new_node;
}

SList slist_remove_head(SList head) {
    if (slist_empty(head))
        return NULL;

    SList tmp = head->next;
    free(head);
    return tmp;
}

unsigned slist_len(SList head) {
    size_t len;
    for (len = 0; head != NULL; len++, head = head->next);
    return len;
}

void slist_free(SList head) {
    SList tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void slist_print(SList head) {
    printf("[ ");
    for (; head != NULL; head = head->next)
        printf("%d, ", head->data);
    printf("]\n");
}
