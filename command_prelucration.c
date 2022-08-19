// Copyright 2022 Ilinca Sebastian-Ionut

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bible.h"

/* Allocate dinamycally size for s which is a string */
char *
allocate_size(char *s)
{
    s = malloc(string_size);
    if (!s) {
        m_error;
        return NULL;
    }
    return s;
}

/* Delete "\n" if it is present in s */
void
remove_basic_newline(char *s)
{
    if (strlen(s) > 0 && s[strlen(s) - 1] == '\n')
        s[strlen(s) - 1] = '\0';
}
