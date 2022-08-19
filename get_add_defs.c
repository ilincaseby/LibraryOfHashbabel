// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Give me the whole command block, I will separate
between them and search in the memory for the value
of definition master */
void
get_and_print_def(hashtable_t *library, char *command, char *buffer)
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

    // verify is the book and definition is there
    if (ht_has_key(library, name_of_book) == 0) {
        no_book;
        free(name_of_book);
        free(definition);
        return;
    }
    ll_node_t *book = get_book_wout_extract(library, name_of_book);
    info *book_content = (info *) book->data;
    hashtable_t *content = (hashtable_t *) book_content->value;
    if (ht_has_key(content, definition) == 0) {
        no_def;
		free(name_of_book);
        free(definition);
        return;
    }

    // reuse functions because that get_book works with definitions
    // too. The thing is that the function works with generic, so it's fine
    ll_node_t *def = get_book_wout_extract(content, definition);
    info *data_from_def = (info *) def->data;
    char *value_of_def = (char *) data_from_def->value;
    printf("%s\n", value_of_def);
    free(name_of_book);
    free(definition);
}

/* Given a library, a command block and a buffer,
this function add a definition to a book if there is that book */
void
add_def_to_book(hashtable_t *library, char *command, char *buffer)
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
	char *value_of_def = malloc(string_size);
	if (!value_of_def) {
		m_error;
		return;
	}
	buf = strtok(NULL, " ");
	strcpy(value_of_def, buf);

	/* Verify the existence of the book */
	if (ht_has_key(library, name_of_book) == 0) {
		no_book;
		free(name_of_book);
		free(definition);
		free(value_of_def);
		return;
	}

	/* Get book and cast info's until I arrive at hashtable */
	ll_node_t *book = get_book_wout_extract(library, name_of_book);
	info *content_book = (info *) book->data;
	hashtable_t *content = (hashtable_t *) content_book->value;
    if (ht_has_key(content, definition) == 0)
	    ht_put(content, definition, strlen(definition) + 1,
            value_of_def, strlen(value_of_def) + 1);
    if (ht_has_key(content, definition) == 1) {
        ll_node_t *for_change = get_book_wout_extract(content, definition);
        info *pair = (info *) for_change->data;
        free(pair->value);
        pair->value = malloc(strlen(value_of_def) + 1);
        if (!pair->value) {
            m_error;
            return;
        }
        memcpy(pair->value, value_of_def, strlen(value_of_def) + 1);
    }

	/* I do the resize, but because content may point to something else the
	idea is that I try make the value pointing to that new block.
	First: If the resize has been made, the old block of memory is now freed.
	Second: Content now points to the new block, but hashtable from lib in book
	is the same, that means basically I lose the old memory with all defs and
	obviously after the end of this function I don't know where in memory is my
	new block.
	I stayed some time to think about this:))).*/
	calculate_load_factor_and_decide_for_books(&content);
	content_book->value = (void *) content;

	/* Free allocated memory */
	free(name_of_book);
	free(definition);
	free(value_of_def);
}
