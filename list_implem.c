// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Create a linked list */
linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));
    if (!ll) {
		m_error;
		return NULL;
	}

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

/* Add node to a list with the index n, if n > size then
the node will be added to the end of the linked list */
void
ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    /* n >= list->size means adding at last. */
    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    if (!new_node) {
		m_error;
		return;
	}
    new_node->data = malloc(list->data_size);
    if (!new_node->data) {
		m_error;
		return;
	}
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        /* n == 0. */
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

/* Function to remove a certain node from the list
WARNING! IT'S YOUR JOB TO FREE IT */
ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    /* n >= list->size - 1 means delete last. */
    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        /* n == 0. */
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

/* Return a node which is part of the list and
indexed by index */
ll_node_t *
get_node(linked_list_t *list, unsigned int index)
{
    if (!list || !list->head)
        return NULL;
    /* n >= list->size - 1 means return last. */
    if (index > list->size - 1) {
        index = list->size - 1;
    }

    ll_node_t *curr = list->head;
    while (index > 0) {
        curr = curr->next;
        --index;
    }
    return curr;
}

/* Get the size of the list */
unsigned int
ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

/* Free the pp_list and make points to the NULL */
void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

/* Print the entire list data if it is int type */
void
ll_print_int(linked_list_t* list)
{
    ll_node_t* curr;

    if (!list) {
        return;
    }

    curr = list->head;
    while (curr != NULL) {
        printf("%d ", *((int*)curr->data));
        curr = curr->next;
    }

    printf("\n");
}

/* Print the entire list data if it is string type */
void
ll_print_string(linked_list_t* list)
{
    ll_node_t* curr;

    if (!list) {
        return;
    }

    curr = list->head;
    while (curr != NULL) {
        printf("%s ", (char*)curr->data);
        curr = curr->next;
    }

    printf("\n");
}
