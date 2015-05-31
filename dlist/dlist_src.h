#include <stdio.h>
#include <stdlib.h>

typedef struct _list_node
{
    zval *value;
    struct _list_node *prev;
    struct _list_node *next;
}dlist_node;

typedef struct _list_head
{
    int size;
    dlist_node *head;
    dlist_node *tail;
}dlist_head;

dlist_head *dlist_create();
int dlist_add_head(dlist_head *head, zval *value);
int dlist_add_tail(dlist_head *head, zval *value);
int dlist_delete_index(dlist_head *head, int index);
int dlist_fetch(dlist_head *head, int index, zval **retval);
int dlist_length(dlist_head *head);
void dlist_destroy(dlist_head *head);































