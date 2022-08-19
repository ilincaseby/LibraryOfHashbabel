// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Print infos about book */
void
get_book_print(hashtable_t *library, char *book_name)
{
    if (ht_has_key(library, book_name) == 0) {
        no_book;
        return;
    }
    hashtable_t *book = ht_get(library, book_name);
    char rating[21] = "c3VtX29mX3JhdGluZw==";
    int f_rating = atoi(ht_get(book, rating));
    char purchases[41] = "aG93X21hbnlfdGltZXNfaXNfdGhlX2Jvb2tfcHVy";
    int i_purc = atoi(ht_get(book, purchases));
    float total = 0;
    if (i_purc > 0) {
        total = (float) f_rating / (float) i_purc;
    }
    printf("Name:%s Rating:%.3f Purchases:%d\n", book_name, total, i_purc);
}
