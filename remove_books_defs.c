// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Get the pointer to the book and remove it from the list
WARNING! YOU NEED TO BE SURE IF THE LIBRARY HAS THE KEY */
ll_node_t *
get_book(hashtable_t *library, void *book)
{
    unsigned int index = 0;
    ll_node_t *desired_book;
    unsigned int where_should_be = library->hash_function(book);
    where_should_be %= library->hmax;
    linked_list_t *list_of_books = library->buckets[where_should_be];
    desired_book = list_of_books->head;

    for (unsigned int i = 0; i < list_of_books->size; ++i) {
        info *info_bout_book = (info *) desired_book->data;
        int is_this = library->compare_function(book, info_bout_book->key);
        if (is_this == 0) {
            index = i;
            break;
        }
        desired_book = desired_book->next;
    }
    desired_book = ll_remove_nth_node(list_of_books, index);
    if (list_of_books->size == 0)
        library->size--;
    return desired_book;
}

/* Remove a book */
void
remove_book(hashtable_t *library, char *command, char *buffer)
{
    // initiate the book name
    strcpy(buffer, command);
    char *buf;
    buf = buffer + 9;
	buf = strtok(buf, "\"");
    char *name_of_book = malloc(string_size);
    if (!name_of_book) {
        m_error;
        return;
    }
    strcpy(name_of_book, buf);

    // verify the book
    if (ht_has_key(library, name_of_book) == 0) {
        no_book;
        free(name_of_book);
        return;
    }
    // take it out and free the memory allocated
    ll_node_t *book = get_book(library, name_of_book);
    info *book_info = (info *) book->data;
    hashtable_t *content = (hashtable_t *) book_info->value;
    ht_free(content);
    free(book_info->key);
    free(book->data);
    free(book);
    free(name_of_book);
}

/* Get a pointer to the book without taking it out extracting it */
ll_node_t *
get_book_wout_extract(hashtable_t *library, void *book)
{
    ll_node_t *desired_book;
    unsigned int where_should_be = library->hash_function(book);
    where_should_be %= library->hmax;
    linked_list_t *list_of_books = library->buckets[where_should_be];
    desired_book = list_of_books->head;

    for (unsigned int i = 0; i < list_of_books->size; ++i) {
        info *info_bout_book = (info *) desired_book->data;
        int is_this = library->compare_function(book, info_bout_book->key);
        if (is_this == 0)
            break;
        desired_book = desired_book->next;
    }
    return desired_book;
}

/* Remove a definition in a book
NOTE: The book's and def's name are in the command string */
void
remove_definition(hashtable_t *library, char *command, char *buffer)
{
    // initiate the names of book and definition
    strcpy(buffer, command);
    char *buf = buffer + 8;
	buf = strtok(buf, "\"");
    char *name_of_book = malloc(string_size);
    char *definition = malloc(string_size);
    if (!definition || !name_of_book) {
        m_error;
        return;
    }
    strcpy(name_of_book, buf);
    buf = strtok(NULL, " \"");
    strcpy(definition, buf);
    if (ht_has_key(library, name_of_book) == 0) {
        no_book;
        free(name_of_book);
        free(definition);
        return;
    }
    // take the pointer of book without taking it out
    ll_node_t *book = get_book_wout_extract(library, name_of_book);

    // for economy, after verify, I reuse the function which actually
    // extract the node, it is a generic function, so I can use it
    // everywhere with a hashtable, just the vars are names suggestive
    info *book_content = (info *) book->data;
    hashtable_t *content = (hashtable_t *) book_content->value;
    if (ht_has_key(content, definition) == 0) {
        no_def;
        free(name_of_book);
        free(definition);
        return;
    }
    ll_node_t *def = get_book(content, definition);
    info *def_content = (info *) def->data;
    free(def_content->key);
    free(def_content->value);
    free(def->data);
    free(def);
    free(name_of_book);
    free(definition);
}
