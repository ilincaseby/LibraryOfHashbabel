// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

void
notify_lost(hashtable_t *library, hashtable_t *users,
		char *command, char *buffer)
{
	strcpy(buffer, command);
	char *buf = strtok(buffer, " ");
	char *book_name = malloc(string_size), *username = malloc(string_size);
	if (!book_name || !username) {
		m_error;
		return;
	}
	buf = strtok(NULL, " ");
	strcpy(username, buf);
	buf = strtok(NULL, "\"");
	strcpy(book_name, buf);
	if (ht_has_key(users, username) == 0) {
		printf("You are not registered yet.\n");
		free(username);
		free(book_name);
		return;
	}
	user_features *feats = (user_features *) ht_get(users, username);
	if (*(feats->banned) == true) {
		printf("You are banned from this library.\n");
		free(username);
		free(book_name);
		return;
	}
	ll_node_t *book = get_book(library, book_name);
	info *book_info = (info *) book->data;
	hashtable_t *book_content = (hashtable_t *) book_info->value;
	ht_free(book_content);
	free(book_info->key);
	free(book->data);
	free(book);
	*(feats->borrowed) = false;
	free(feats->book_borrowed);
	free(feats->days);
	int points = *(int *) feats->points;
	points -= ban_p;
	if (points < 0) {
		*(feats->banned) = true;
		printf("The user %s has been banned from this library.\n", username);
	}
	*(int *) feats->points = points;
	free(username);
	free(book_name);
}
