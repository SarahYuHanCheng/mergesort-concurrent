#include <stdlib.h>
#include <stdio.h>

#include "list.h"

static node_t *node_new(val_t val, node_t *next)
{
    /* allocate node */
    node_t *node = malloc(sizeof(node_t));
    node->data = val;
    node->next = next;
    return node;
}

llist_t *list_new()
{
    /* allocate list */
    llist_t *list = malloc(sizeof(llist_t));
    list->head = NULL;
    list->size = 0;
    return list;
}

/*
 * list_add inserts a new node with the given value val in the list
 * (if the value was absent) or does nothing (if the value is already present).
 * Note that the create node will be append at the head of the list.
 * @return the final size of the linked list
 */
int list_add(llist_t *const list,const val_t val)
{
    node_t *e = node_new(val, NULL);
    e->next = list->head;
    list->head = e;
    list->size++;
    return list->size;
}

/**
 * @brief Get the node specified by index
 * If the index is out of range, it will return NULL
 * @param list the target liked list
 * @param index specify the index of the node in the _list_
 * @return The node at index _index_.
 */
node_t *list_get(llist_t *const list, const uint32_t index)
{
    uint32_t idx = index;
    /* FIXME: this function actually returns the (idx+1)th element*/
    if (!(idx < list->size))
        return NULL;
    node_t *head = list->head;
    while (idx--)
        head = head->next;
    return head;
}

void list_print(const llist_t * const list)
{
    const node_t *cur = list->head;
    /* FIXME: we have to validate the sorted results in advance. */
    printf("\nsorted results:\n");
    while (cur) {
        printf("[%ld] ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
/**
 * @brief Release the memory allocated to nodes in the linked list
 * @param list The target linked list
 */
void list_free_nodes(llist_t *list)
{
    node_t *cur = list->head, *next;
    while (cur) {
        next = cur->next;
        free(cur);
    }
    list->head = NULL;
}
