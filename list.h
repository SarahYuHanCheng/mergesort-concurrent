#ifndef LLIST_H_
#define LLIST_H_

#include <stdint.h>

typedef intptr_t val_t;

typedef struct node {
    val_t data;
    struct node *next;
} node_t;
/* Store the information of the linked list.*/
typedef struct llist {
    node_t *head; /* the head of the linked list*/
    uint32_t size;/* the size of the linked list*/
} llist_t;

llist_t *list_new();
int list_add(llist_t *the_list, val_t val);
void list_print(llist_t *the_list);
node_t *list_get(llist_t *the_list, uint32_t index);

#endif
