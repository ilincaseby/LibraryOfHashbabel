// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Initialise a struct user_features to help the code to
be more readable */
user_features *
init_user_feats(user_features *user)
{
    user = malloc(sizeof(*user));
    if (!user) {
        m_error;
        return NULL;
    }
    user->points = malloc(sizeof(int));
    if (!user->points) {
        m_error;
        return NULL;
    }
    user->borrowed = malloc(sizeof(bool));
    if (!user->borrowed) {
        m_error;
        return NULL;
    }
    user->banned = malloc(sizeof(bool));
    if (!user->banned) {
        m_error;
        return NULL;
    }
    *(user->points) = 100;
    *(user->banned) = false;
    *(user->borrowed) = false;
    return user;
}

/* Being given the whole block of command, this function
separate arguments and make the user add more easy than on paper */
void
add_user_in_system(hashtable_t *users, char *command, char *buffer)
{
    char *username = malloc(string_size);
    if (!username) {
        m_error;
        return;
    }
    strcpy(buffer, command);
    char *buf = strtok(buffer, " ");
    buf = strtok(NULL, " ");
    strcpy(username, buf);
    if (ht_has_key(users, username) == 1) {
        user_is_reg;
        free(username);
        return;
    }
    user_features *user = NULL;
    user = init_user_feats(user);
    ht_put(users, username, strlen(username) + 1, user, sizeof(user_features));
    free(username);
    free(user);
}
