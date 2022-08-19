// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

int main()
{
    char *command = NULL, *buffer = NULL;
    /* Pointer to function which helps string to ocuppy memory */
    char *(*access_to_read_and_write)(char *) = &allocate_size;
    command = (*access_to_read_and_write)(command);
    buffer = (*access_to_read_and_write)(buffer);
    hashtable_t *library = create_me;
    hashtable_t *users = create_me;

    while (1) {
        fgets(command, string_size, stdin);
        remove_basic_newline(command);
        if (strncmp(command, "ADD_BOOK", 8) == 0) {
            add_book(command, buffer, library);
            calculate_load_factor_and_decide(&library);
        } else if (strncmp(command, "RMV_BOOK", 8) == 0) {
            remove_book(library, command, buffer);
        } else if (strncmp(command, "RMV_DEF", 7) == 0) {
            remove_definition(library, command, buffer);
        } else if (strncmp(command, "GET_DEF", 7) == 0) {
            get_and_print_def(library, command, buffer);
        } else if (strncmp(command, "ADD_DEF", 7) == 0) {
            add_def_to_book(library, command, buffer);
        } else if (strncmp(command, "ADD_USER", 8) == 0) {
            add_user_in_system(users, command, buffer);
            load_factor_decide_for_users(&users);
        } else if (strncmp(command, "BORROW", 6) == 0) {
            book_borrow(library, users, command);
        } else if (strncmp(command, "LOST", 4) == 0) {
            notify_lost(library, users, command, buffer);
        } else if (strncmp(command, "RETURN", 6) == 0) {
            return_book(library, users, command, buffer);
        } else if (strncmp(command, "GET_BOOK", 8) == 0) {
            strcpy(buffer, command);
            char *buf = strtok(buffer, " ");
            buf = strtok(NULL, "\"");
            get_book_print(library, buf);
        } else if (strncmp(command, "EXIT", 4) == 0) {
            resize_hash_at_exit(&library);
            users = resize_hash_for_users_exit(users, 2.000);
            library_in_flames(library, users);
            ht_free(library);
	        ht_free(users);
            break;
        }
    }
    free(command);
    free(buffer);
    return 0;
}
