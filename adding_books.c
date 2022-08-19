// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Adding a book */
void
add_book(char *command, char *buffer, hashtable_t *library)
{
	// initiate the name of the book
	// and the number of definition
    strcpy(buffer, command);
	char *buf = buffer + 9;
	buf = strtok(buf, "\"");
	char *name_of_book = malloc(string_size);
    if (!name_of_book) {
        m_error;
        return;
    }
	strcpy(name_of_book, buf);
	buf = strtok(NULL, " \"");
	unsigned int def_number = atoi(buf);
	// create a hashtable and initiate it with the definitions
	if (ht_has_key(library, name_of_book) == 1) {
		ll_node_t *book_to_free = get_book(library, name_of_book);
		info *info_book = book_to_free->data;
		hashtable_t *book_hash = (hashtable_t *) info_book->value;
		ht_free(book_hash);
		free(info_book->key);
		free(book_to_free->data);
		free(book_to_free);
	}
	hashtable_t *book = ht_create(first_max_b,
	hash_function_string, compare_function_strings);
	char *def_key = malloc(string_size);
	char *def_val = malloc(string_size);
	if (!def_key || !def_val) {
		m_error;
		return;
	}
	for (unsigned int i = 0; i < def_number; ++i) {
		fgets(command, string_size, stdin);
		remove_basic_newline(command);
		strcpy(buffer, command);
		buf = strtok(buffer, " ");
		strcpy(def_key, buf);
		buf = strtok(NULL, " ");
		strcpy(def_val, buf);
		if (ht_has_key(book, def_key) == 1) {
			ll_node_t *for_change = get_book_wout_extract(book, def_key);
			info *pair = (info *) for_change->data;
			free(pair->value);
			pair->value = malloc(strlen(def_val) + 1);
			if (!pair->value) {
				m_error;
				return;
			}
			memcpy(pair->value, def_val, strlen(def_val) + 1);
		}
		ht_put(book, def_key, strlen(def_key) + 1, def_val, strlen(def_val) + 1);
		calculate_load_factor_and_decide_for_books(&book);
	}

	// I add some names in the book for the purchases, rating and borrowing

	// base64 for how_many_times_is_the_book_purchased
	char purchases[41] = "aG93X21hbnlfdGltZXNfaXNfdGhlX2Jvb2tfcHVy";
	// "Y2hhc2Vk" => these remained from encoding ^
	// base64 for sum_of_rating
	char rating[21] = "c3VtX29mX3JhdGluZw==";
	// base64 for is_the_book_borrowed
	char borrow[29] = "aXNfdGhlX2Jvb2tfYm9ycm93ZWQ=";

	// for values I don't need encoding
	char no_purchases[2] = "0";
	char no_rating[2] = "0";
	char no_borrow[5] = "fals";

	ht_put(book, purchases, strlen(purchases) + 1,
		no_purchases, strlen(no_purchases) + 1);
	calculate_load_factor_and_decide_for_books(&book);
	ht_put(book, rating, strlen(rating) + 1, no_rating, strlen(no_rating) + 1);
	calculate_load_factor_and_decide_for_books(&book);
	ht_put(book, borrow, strlen(borrow) + 1, no_borrow, strlen(no_borrow) + 1);
	calculate_load_factor_and_decide_for_books(&book);

	// put near the name of the book the value of pointer which holds
	// the information that book handle
	ht_put(library, name_of_book, strlen(name_of_book) + 1,
		book, sizeof(hashtable_t));
	free(name_of_book);
	free(def_key);
	free(def_val);
	free(book);
}
