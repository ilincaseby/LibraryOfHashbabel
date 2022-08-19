// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "bible.h"

/* Compare two void pointers casting them to int
0 - Identical
1 - N/I */
int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

/* Compare two void pointers casting them to char
0 - Identical
1 - N/I */
int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	int difference = strcmp(str_a, str_b);
	return difference;
}

/* Return a number resulted from a hash in case if the
key is actually int type */
unsigned int
hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

/* Return a number resulted from a hash in case if the key
is string type */
unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	uint64_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

/* Create a hashtable */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t *h_table = malloc(sizeof(*h_table));

	if (!h_table) {
		m_error;
		return NULL;
	}
	h_table->hmax = hmax;
	h_table->size = 0;
	h_table->hash_function = (hash_function);
	h_table->compare_function = (compare_function);

	h_table->buckets = malloc(hmax * sizeof(*(h_table->buckets)));
	if (!h_table->buckets) {
		m_error;
		return NULL;
	}

	for (unsigned int i = 0; i < hmax; ++i) {
		h_table->buckets[i] = ll_create(sizeof(info));
	}
	return h_table;
}

/* Verify if ht and key exists */
int
verify_ver(hashtable_t *ht, void *key)
{
	if (!ht)
		return 0;
	if (!key)
		return 0;
	return 1;
}

/*
 * Function that returns:
 * 1, if the key already exists
 * 0, it doesn't
 */
int
ht_has_key(hashtable_t *ht, void *key)
{
	int ver = verify_ver(ht, key);
	if (ver == 0)
		return 0;
	unsigned int hash = ht->hash_function(key);
	hash %= ht->hmax;
	linked_list_t *list_for_verify = ht->buckets[hash];
	int stop = list_for_verify->size;
	ll_node_t *this_is_it = list_for_verify->head;
	int verify;

	for (int i = 0; i < stop; ++i) {
		info *to_key = ((info*)(this_is_it->data));
		verify = ht->compare_function(to_key->key, key);
		if (verify == 0) {
			return 1;
		}
		this_is_it = this_is_it->next;
	}

	return 0;
}

/* Return the value */
void *
ht_get(hashtable_t *ht, void *key)
{
	if (!ht)
		return NULL;
	int verify = ht_has_key(ht, key);
	if(verify == 0)
		return NULL;
	// small chances but who knows
	if(!key)
		return NULL;
	unsigned int hashy = ht->hash_function(key);
	hashy %= ht->hmax;
	linked_list_t *list = ht->buckets[hashy];
	ll_node_t *node = list->head;
	verify = 1;
	while (verify) {
		info *k_v = (info *) node->data;
		verify = ht->compare_function(k_v->key, key);

		if (!verify) {
			return k_v->value;
		}

		node = node->next;
	}

	return NULL;
}

/* Put the value and key to the hashtable */
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	int have_been_there = ht_has_key(ht, key);
	if (have_been_there) {
		void *value_to_be_changed = ht_get(ht, key);
		memmove(value_to_be_changed, value, value_size);
		return;
	}

	info *data = malloc(sizeof(*data));
	if (!data) {
		m_error;
		return;
	}
	data->key = malloc(key_size);
	if (!data->key) {
		m_error;
		return;
	}
	data->value = malloc(value_size);
	if (!data->value) {
		m_error;
		return;
	}

	memmove(data->key, key, key_size);
	memmove(data->value, value, value_size);

	unsigned int hashy = ht->hash_function(key);
	hashy %= ht->hmax;
	linked_list_t *list = ht->buckets[hashy];
	if (list->size == 0)
		ht->size++;
	ll_add_nth_node(list, list->size, ((void *) data));
	free(data);
}


/* 0 - You can't delete
1 - You can delete */
int
can_I_delete(hashtable_t *ht, void *key)
{
	if (!ht)
		return 0;
	if (!key)
		return 0;
	if (ht_has_key(ht, key) == 0)
		return 0;
	return 1;
}

/* Remove the node hold by key */
void
ht_remove_entry(hashtable_t *ht, void *key)
{
	int can_you = can_I_delete(ht, key);
	if (!can_you)
		return;
	unsigned int hashy = ht->hash_function(key);
	hashy %= ht->hmax;
	linked_list_t *my_list = ht->buckets[hashy];
	ll_node_t *node = NULL;
	node = my_list->head;
	int node_index;
	for (int i = 0; i < (int) my_list->size; ++i) {
		info *conserved_data = (info *) node->data;
		if (!(ht->compare_function(conserved_data->key, key))) {
			node_index = i;
			break;
		}
		node = node->next;
	}
	node = ll_remove_nth_node(my_list, (unsigned int) node_index);
	info *data_to_be_removed = (info *) node->data;
	free(data_to_be_removed->key);
	free(data_to_be_removed->value);
	free(node->data);
	free(node);
	if (my_list->size == 0)
		ht->size--;
}

/* Free hashtable */
void
ht_free(hashtable_t *ht)
{
	int stop = (int) ht->hmax;
	linked_list_t *list = NULL;
	ll_node_t *node = NULL;
    for (int i = 0; i < stop; ++i) {
		list = ht->buckets[i];
		node = list->head;
		while (list->size != 0) {
			node = ll_remove_nth_node(list, 0);
			info *data_to_be_removed = (info *) node->data;
			free(data_to_be_removed->key);
			free(data_to_be_removed->value);
			free(node->data);
			free(node);
			if (list->size == 0)
				ht->size--;
		}
		free(ht->buckets[i]);
	}
	free(ht->buckets);
	free(ht);
}
/* Returns the number of buckets occupied */
unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

/* Returns the maximum size of buckets */
unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
