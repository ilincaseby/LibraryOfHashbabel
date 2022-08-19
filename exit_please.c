// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

float
get_rating(hashtable_t *book)
{
    char rating[21] = "c3VtX29mX3JhdGluZw==";
    char purchases[41] = "aG93X21hbnlfdGltZXNfaXNfdGhlX2Jvb2tfcHVy";
	float q_rating = atof(ht_get(book, rating));
	int purc = atoi(ht_get(book, purchases));
	float total = 0;
	if (!purc) {
		return total;
	}
	total = q_rating / (float) purc;
	return total;
}

ll_node_t *
max_book(linked_list_t *buck)
{
    ll_node_t *max = buck->head;
    info *max_c = (info *) max->data;
    hashtable_t *cont = (hashtable_t *) max_c->value;
    float maximum = get_rating(cont);
	ll_node_t *aux = max;
	unsigned int index = 0;
	for (unsigned int i = 1; i < buck->size; ++i) {
		aux = aux->next;
		info *aux_c = (info *) aux->data;
		hashtable_t *a_cont = (hashtable_t *) aux_c->value;
		float comparison = get_rating(a_cont);
		if (comparison > maximum) {
			index = i;
			maximum = comparison;
		}
	}
	max = ll_remove_nth_node(buck, index);
	return max;
}

ll_node_t *
max_user(linked_list_t *user)
{
	ll_node_t *max = user->head;
	ll_node_t *aux = max;
	info *max_c = (info *) max->data;
	user_features *cont = (user_features *) max_c->value;
	int points = *(cont->points);
	unsigned int index = 0;
	for (unsigned int i = 1; i < user->size; ++i) {
		aux = aux->next;
		info *aux_c = (info *) aux->data;
		user_features *a_cont = (user_features *) aux_c->value;
		int comp = *(a_cont->points);
		if (comp > points) {
			index = i;
			points = comp;
		}
	}
	max = ll_remove_nth_node(user, index);
	return max;
}

void
put_atrib_book(book_atrib *book_v, unsigned int bucks_lib, hashtable_t *library)
{
	int condition_to_sub = 0;
	unsigned int index = 0;
	for (unsigned int i = 0; i < bucks_lib; ++i) {
		linked_list_t *list = library->buckets[i];
		while (list->size) {
			condition_to_sub = 1;
			ll_node_t *node = ll_remove_nth_node(list, 0);
			info *book = (info *) node->data;
			book_v[index].name = malloc(strlen((char *) book->key) + 1);
			char purchases[41] = "aG93X21hbnlfdGltZXNfaXNfdGhlX2Jvb2tfcHVy";
			char rating[21] = "c3VtX29mX3JhdGluZw==";
			memcpy
			(book_v[index].name, book->key, strlen((char *) book->key) + 1);

			int purc = atoi
			((char *) ht_get((hashtable_t *) book->value, purchases));

			int rat = atoi
			((char *) ht_get((hashtable_t *) book->value, rating));

			float average = 0;
			book_v[index].purchases = purc;

			if (purc) {
				average = (float) rat / (float) purc;
			}
			book_v[index].rating = average;

			ht_free((hashtable_t *) book->value);
			free(book->key);
			free(node->data);
			free(node);
			++index;
		}
		if (condition_to_sub)
			library->size--;
	}

	for (unsigned int i = 0; i < index - 1; ++i) {
		for (unsigned int j = i + 1; j < index; ++j) {
			switch_if_the_case(book_v, i, j);
		}
	}
}

void
switch_if_the_case(book_atrib *book_v, int i, int j)
{
	float first = book_v[i].rating;
	float second = book_v[j].rating;
	int st_pur = book_v[i].purchases;
	int nd_pur = book_v[j].purchases;
	int lexico = strcmp(book_v[i].name, book_v[j].name);
	if (first < second) {
		char *aux = book_v[i].name;
		book_v[i].name = book_v[j].name;
		book_v[j].name = aux;
		float r_a = book_v[i].rating;
		book_v[i].rating = book_v[j].rating;
		book_v[j].rating = r_a;
		int p_u = book_v[i].purchases;
		book_v[i].purchases = book_v[j].purchases;
		book_v[j].purchases = p_u;
	}
	if (first == second && st_pur < nd_pur) {
		char *aux = book_v[i].name;
		book_v[i].name = book_v[j].name;
		book_v[j].name = aux;
		float r_a = book_v[i].rating;
		book_v[i].rating = book_v[j].rating;
		book_v[j].rating = r_a;
		int p_u = book_v[i].purchases;
		book_v[i].purchases = book_v[j].purchases;
		book_v[j].purchases = p_u;
	}
	if (first == second && st_pur == nd_pur && lexico > 0) {
		char *aux = book_v[i].name;
		book_v[i].name = book_v[j].name;
		book_v[j].name = aux;
		float r_a = book_v[i].rating;
		book_v[i].rating = book_v[j].rating;
		book_v[j].rating = r_a;
		int p_u = book_v[i].purchases;
		book_v[i].purchases = book_v[j].purchases;
		book_v[j].purchases = p_u;
	}
}

void
put_atrib_user(user_atrib *user_v, unsigned int bucks_users, hashtable_t *users)
{
	int cond_to_sub = 0;
	unsigned int index = 0;
	for (unsigned int i = 0; i < bucks_users; ++i) {
		linked_list_t *list = users->buckets[i];
		while(list->size) {
			cond_to_sub = 1;
			ll_node_t *node = ll_remove_nth_node(list, 0);
			info *user_info = (info *) node->data;
			user_v[index].name = malloc(strlen((char *) user_info->key) + 1);
			if (!user_v[index].name) {
				m_error;
				return;
			}
			memcpy(user_v[index].name, user_info->key,
				strlen((char *) user_info->key) + 1);
			user_features *feats = (user_features *) user_info->value;
			user_v[index].points = * (int *) feats->points;
			free(user_info->key);
			if (*(feats->borrowed) == true) {
				free(feats->book_borrowed);
				free(feats->days);
			}
			free(feats->borrowed);
			free(feats->points);
			free(feats->banned);
			free(user_info->value);
			free(user_info);
			free(node);
			++index;
		}
		if (cond_to_sub)
			users->size--;
	}

	for (unsigned int i = 0; i < index - 1; ++i) {
		for (unsigned int j = i + 1; j < index; ++j) {
			if (user_v[j].points > user_v[i].points) {
				int aux = user_v[i].points;
				char *name_aux = user_v[i].name;
				user_v[i].points = user_v[j].points;
				user_v[i].name = user_v[j].name;
				user_v[j].points = aux;
				user_v[j].name = name_aux;
			}
			if (strcmp(user_v[i].name, user_v[j].name) > 0 &&
				user_v[j].points == user_v[i].points) {
				int aux = user_v[i].points;
				char *name_aux = user_v[i].name;
				user_v[i].points = user_v[j].points;
				user_v[i].name = user_v[j].name;
				user_v[j].points = aux;
				user_v[j].name = name_aux;
			}
		}
	}
}

void
library_in_flames(hashtable_t *library, hashtable_t *users)
{
	// exit if nothing is there
	unsigned int bucks_lib = library->hmax;
	unsigned int elems_lib = 0;
	for (unsigned int i = 0; i < bucks_lib; ++i) {
		linked_list_t *bucket = library->buckets[i];
		elems_lib += bucket->size;
	}

	unsigned int bucks_users = users->hmax;
	unsigned int elems_users = 0;
	for (unsigned int i = 0; i < bucks_users; ++i) {
		linked_list_t *bucket = users->buckets[i];
		elems_users += bucket->size;
	}
	printf("Books ranking:\n");
	if (elems_lib) {
		book_atrib *book_v = malloc(elems_lib * sizeof(*book_v));
		if (!book_v) {
			m_error;
			return;
		}
		put_atrib_book(book_v, bucks_lib, library);

		for (unsigned int i = 0; i < elems_lib; ++i) {
			printf("%u. Name:%s Rating:%.3f Purchases:%d\n", i + 1,
				book_v[i].name, book_v[i].rating, book_v[i].purchases);
			free(book_v[i].name);
		}
		free(book_v);
	}

	printf("Users ranking:\n");
	if (elems_users) {
		user_atrib *user_v = malloc(elems_users * sizeof(*user_v));
		if (!user_v) {
			m_error;
			return;
		}
		put_atrib_user(user_v, bucks_users, users);

		for (unsigned int i = 0; i < elems_users; ++i) {
			if (user_v[i].points >= 0) {
				printf("%u. Name:%s Points:%d\n", i + 1, user_v[i].name, user_v[i].points);
			}
			free(user_v[i].name);
		}
		free(user_v);
	}
}
