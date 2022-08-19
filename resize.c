// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Calculate "fullness" of hashtable and send parameters
to resize_hash */
void
calculate_load_factor_and_decide_for_books(hashtable_t **hash)
{
	unsigned int B = (*hash)->hmax;
	unsigned int E = 0;
	for (unsigned int i = 0; i < B; ++i) {
		E += (*hash)->buckets[i]->size;
	}
	float load_factor = (float) E / (float) B;
	if (load_factor <= 1)
		return;
	*hash = resize_hash_for_books(*hash, load_factor);
}

/* Being given a hashtable & a load_factor this function
resize the hashtable if it needs that */
hashtable_t *
resize_hash_for_books(hashtable_t *hash, float load_factor)
{
	if (load_factor <= 1)
		return hash;
	hashtable_t *resized = ht_create((hash->hmax * 2),
			hash->hash_function, hash->compare_function);
	linked_list_t *buck = NULL;
	ll_node_t *node = NULL;
	for (unsigned int i = 0; i < hash->hmax; ++i) {
		buck = hash->buckets[i];
		for (unsigned int j = 0; j < buck->size; ++j) {
			node = get_node(buck, j);
			info *key_value = (info *) node->data;
			char *key = (char *) key_value->key;
			char *value = (char *) key_value->value;
			ht_put(resized, key_value->key, strlen(key) + 1,
				key_value->value, strlen(value) + 1);
		}
	}
	ht_free(hash);
	return resized;
}

/* Calculate for the entire library */
void
calculate_load_factor_and_decide(hashtable_t **hash)
{
	unsigned int B = (*hash)->hmax;
	unsigned int E = 0;
	for (unsigned int i = 0; i < B; ++i) {
		E += (*hash)->buckets[i]->size;
	}
	float load_factor = (float) E / (float) B;
	*hash = resize_hash(*hash, load_factor);
}

/* Resize for the entire library */
hashtable_t *
resize_hash(hashtable_t *hash, float load_factor)
{
    if (load_factor <= 1)
		return hash;
	hashtable_t *resized = ht_create((hash->hmax * 2),
			hash->hash_function, hash->compare_function);
	linked_list_t *buck = NULL;
	ll_node_t *node = NULL;
	for (unsigned int i = 0; i < hash->hmax; ++i) {
		buck = hash->buckets[i];
		for (unsigned int j = 0; j < buck->size; ++j) {
			node = get_node(buck, j);
			info *key_value = (info *) node->data;
			char *key = (char *) key_value->key;
			ht_put(resized, key_value->key, strlen(key) + 1,
				key_value->value, sizeof(hashtable_t));
		}
	}
	ht_free(hash);
	return resized;
}

void
load_factor_decide_for_users(hashtable_t **users)
{
	unsigned int B = (*users)->hmax;
	unsigned int E = 0;
	for (unsigned int i = 0; i < B; ++i) {
		E += (*users)->buckets[i]->size;
	}
	float load_factor = (float) E / (float) B;
	*users = resize_hash_for_users(*users, load_factor);
}

hashtable_t *
resize_hash_for_users(hashtable_t *users, float load_factor)
{
	if (load_factor <= 1)
		return users;
	hashtable_t *resized = ht_create((users->hmax * 2),
			users->hash_function, users->compare_function);
	linked_list_t *buck = NULL;
	ll_node_t *node = NULL;
	for (unsigned int i = 0; i < users->hmax; ++i) {
		buck = users->buckets[i];
		for (unsigned int j = 0; j < buck->size; ++j) {
			node = get_node(buck, j);
			info *key_value = (info *) node->data;
			char *key = (char *) key_value->key;
			ht_put(resized, key_value->key, strlen(key) + 1,
				key_value->value, sizeof(user_features));
		}
	}
	ht_free(users);
	return resized;
}

/* Make it to one to be more easy to work wiht
when I print */
void
resize_hash_at_exit(hashtable_t **hash)
{
	hashtable_t *resized = ht_create(1,
		(*hash)->hash_function, (*hash)->compare_function);
	linked_list_t *buck = NULL;
	ll_node_t *node = NULL;
	for (unsigned int i = 0; i < (*hash)->hmax; ++i) {
		buck = (*hash)->buckets[i];
		for (unsigned int j = 0; j < buck->size; ++j) {
			node = get_node(buck, j);
			info *key_value = (info *) node->data;
			char *key = (char *) key_value->key;
			ht_put(resized, key_value->key, strlen(key) + 1,
				key_value->value, sizeof(hashtable_t));
		}
	}
	ht_free(*hash);
	*hash = resized;
}

hashtable_t *
resize_hash_for_users_exit(hashtable_t *users, float load_factor)
{
	if (load_factor <= 1)
		return users;
	hashtable_t *resized = ht_create(1,
			users->hash_function, users->compare_function);
	linked_list_t *buck = NULL;
	ll_node_t *node = NULL;
	for (unsigned int i = 0; i < users->hmax; ++i) {
		buck = users->buckets[i];
		for (unsigned int j = 0; j < buck->size; ++j) {
			node = get_node(buck, j);
			info *key_value = (info *) node->data;
			char *key = (char *) key_value->key;
			ht_put(resized, key_value->key, strlen(key) + 1,
				key_value->value, sizeof(user_features));
		}
	}
	ht_free(users);
	return resized;
}
