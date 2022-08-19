// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

void
my_itoa(int n, char *s)
{
	int len = 0;
	int a = n;
	while(a) {
		a /= 10;
		len++;
	}
	int b = n;
	for (int i = 0; i < len; ++i) {
		a *= 10 + b % 10;
		b /= 10;
	}
	a = n;
	for (int i = 0; i < len; ++i) {
		b = a % 10;
		s[i] = b + '0';
		a /= 10;
	}
	for (int i = 0; i < (len / 2); ++i) {
		char aux = s[i];
		s[i] = s[len - i - 1];
		s[len - i - 1] = aux;
	}
	s[len] = '\0';
}

/* Proccess of borrowing */
void
borrowing_proccess(hashtable_t *library, hashtable_t *users, char *username,
                char *book_name, unsigned int *days)
{
    if (ht_has_key(users, username) == 0) {
        printf("You are not registered yet.\n");
        return;
    }
    ll_node_t *user = get_book_wout_extract(users, username);
    info *user_content = (info *) user->data;
    user_features *feats = (user_features *) user_content->value;
    if (*(feats->banned) == true) {
        printf("You are banned from this library.\n");
        return;
    }

	if (*(feats->borrowed) == true) {
        printf("You have already borrowed a book.\n");
        return;
    }

    if (ht_has_key(library, book_name) == 0) {
        no_book;
        return;
    }

    ll_node_t *book = get_book_wout_extract(library, book_name);
    info *book_content = (info *) book->data;
    hashtable_t *content = (hashtable_t *) book_content->value;

    char string[29] = "aXNfdGhlX2Jvb2tfYm9ycm93ZWQ=";
    ll_node_t *def = get_book_wout_extract(content, string);

    info *def_content = (info *) def->data;

    if (strcmp(def_content->value, "true") == 0) {
        printf("The book is borrowed.\n");
        return;
    }
    char cond[5] = "true";
	memcpy(def_content->value, cond, strlen(cond) + 1);
    feats->days = malloc(sizeof(unsigned int));
    if (!feats->days) {
        m_error;
        return;
    }
    *(feats->days) = *days;
    *(feats->borrowed) = true;
    feats->book_borrowed = malloc(strlen(book_name) + 1);
    if (!feats->book_borrowed) {
        m_error;
        return;
    }
    strcpy(feats->book_borrowed, book_name);
}

/* Help students learn by borrowing them books */
void
book_borrow(hashtable_t *library, hashtable_t *users, char *command)
{
    char *buffer = malloc(string_size);
    if (!buffer) {
        m_error;
        return;
    }
    char *username = malloc(string_size), *book_name = malloc(string_size);
    if (!username || !book_name) {
        m_error;
        return;
    }
    unsigned int *days = malloc(sizeof(unsigned int));
    if (!days) {
        m_error;
        return;
    }
    strcpy(buffer, command);
    char *buf = strtok(buffer, " ");
    buf = strtok(NULL, " ");
    strcpy(username, buf);
    buf = strtok(NULL, "\"");
    strcpy(book_name, buf);
    buf = strtok(NULL, " \"");
    *days = (unsigned int) atoi(buf);
    borrowing_proccess(library, users, username, book_name, days);
    free(buffer);
    free(username);
    free(days);
	free(book_name);
}

/* Return the book */
void
return_book(hashtable_t *library, hashtable_t *users,
        char *command, char *buffer)
{
    strcpy(buffer, command);
    char *buf = strtok(buffer, " ");
    buf = strtok(NULL, " ");
    char *username = malloc(string_size), *book_name = malloc(string_size);
    int days;
    int rating;
    if (!username || !book_name) {
        m_error;
		return;
    }
	strcpy(username, buf);
	buf = strtok(NULL, "\"");
	strcpy(book_name, buf);
	buf = strtok(NULL, " ");
	days = atoi(buf);
	buf = strtok(NULL, " ");
	rating = atoi(buf);
	user_features *feats = (user_features *) ht_get(users, username);


	if (*(feats->banned) == true) {
		printf("You are banned from this library.\n");
		free(username);
		free(book_name);
		return;
	}

	if (*(feats->borrowed) == false) {
		printf("You didn't borrow this book.\n");
		free(username);
		free(book_name);
		return;
	}

	if (strcmp(book_name, feats->book_borrowed)) {
		printf("You didn't borrow this book.\n");
		free(username);
		free(book_name);
		return;
	}

    int days_ment = (int) *(unsigned int *) feats->days;
    int difference = days_ment - days;
    int bonus_points = 0;
    if (difference > 0) {
        bonus_points = difference;
    }
    if (difference < 0) {
        bonus_points = 2 * difference;
    }
    hashtable_t *book = (hashtable_t *) ht_get(library, book_name);
    /* TODO:
    CHANGE THE VALUES OF KEYS THAT HOLDS INFO ABOUT BOOK
    ATTRIBUTE BONUS_POINTS TO THE USER AND MAKE THE DECISION */
	char borrow[29] = "aXNfdGhlX2Jvb2tfYm9ycm93ZWQ=";
	char rating_str[21] = "c3VtX29mX3JhdGluZw==";

	char string2[41] = "aG93X21hbnlfdGltZXNfaXNfdGhlX2Jvb2tfcHVy";
	ll_node_t *def_purc = get_book_wout_extract(book, string2);
	info *purc_info = (info *) def_purc->data;
	int purc = atoi((char *) purc_info->value);
	purc++;
	free(purc_info->value);
	char *ss = malloc(string_size);
	if (!ss) {
		m_error;
		return;
	}
	my_itoa(purc, ss);
	purc_info->value = malloc(strlen(ss) + 1);
	if (!purc_info->value) {
		m_error;
		return;
	}
	strcpy(purc_info->value, ss);
	free(ss);

	char *for_borrow = (char *) ht_get(book, borrow);
	char no_borrow[5] = "fals";
	memcpy(for_borrow, no_borrow, strlen(no_borrow) + 1);
	char *for_rating = (char *) ht_get(book, rating_str);
	int rating_ex = atoi(for_rating);
	rating += rating_ex;
	char *s = malloc(string_size);
	if (!s) {
		m_error;
		return;
	}
	my_itoa(rating, s);
	ll_node_t *rtg = get_book_wout_extract(book, rating_str);
	info *rr = (info *) rtg->data;
	free(rr->value);
	rr->value = malloc(strlen(s) + 1);
	memcpy(rr->value, s, strlen(s) + 1);
	*(feats->points) += bonus_points;
	*(feats->borrowed) = false;
	free(feats->book_borrowed);
	free(feats->days);
	if (*(feats->points) < 0) {
		*(feats->banned) = true;
		printf("The user %s has been banned from this library.\n", username);
	}
	free(username);
	free(book_name);
	free(s);
}

void
my_ftoa(float r, char *s)
{
	int n = (int) r;
	float rest = r - (float) n;
	my_itoa(n, s);
	unsigned int len = strlen(s);
	s[len] = '.';
	rest *= 1000;
	n = (int) rest;
	my_itoa(n, s + len + 1);
}
