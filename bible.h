// Copyright 2022 Ilinca Sebastian-Ionut

#ifndef BIBLE_H_
#define BIBLE_H_

#define string_size 128
#define first_max_b 1
#define ban_p 50
#define no_book printf("The book is not in the library.\n")
#define no_def printf("The definition is not in the book.\n")
#define st_arg first_max_b
#define h_f hash_function_string
#define c_f compare_function_strings
#define create_me ht_create(st_arg, h_f, c_f)
#define m_error fprintf(stderr, "Malloc failed\n")
#define user_is_reg printf("User is already registered.\n")

/* Struct that defines a node of the list
Contains: void *data & struct ll_node_t *next */
typedef struct ll_node_t
{
    void *data;
    struct ll_node_t *next;
} ll_node_t;

/* Struct that defines the list itself
Contains: ll_node_t *head; unsigned int data_size &
unsigned int size */
typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

/* Struct that holds both the key and the value */
typedef struct info {
	void *key; /* Void pointer which contains the key */
	void *value; /* Void pointer which contains the value */
} info;

/* Struct for hashtable */
typedef struct hashtable_t
{
	linked_list_t **buckets; /* Array of linked lists. */
	/* No. of ocuppied buckets */
	unsigned int size;
	unsigned int hmax; /* No. of buckets. */
	/* (Pointer for) a function to make hash happens. */
	unsigned int (*hash_function)(void*);
	/* (Pointer for) a function to compare two keys. */
	int (*compare_function)(void*, void*);
} hashtable_t;

/* Struct for users, this struct will be included in value
of info, as "attributes" */
typedef struct user_features
{
	bool *banned; /* true for ban */
	bool *borrowed; /* true if borrowed */
	unsigned int *days;
	char *book_borrowed;
	int *points;
} user_features;

/* Two structures to help me retain the caracteristiques of users and books to
print them */

typedef struct book_atrib
{
	char *name;
	float rating;
	int purchases;
} book_atrib;

typedef struct user_atrib
{
	char *name;
	int points;
} user_atrib;

/* Functions: */

linked_list_t *
ll_create(unsigned int data_size);

void
ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data);

ll_node_t *
ll_remove_nth_node(linked_list_t *list, unsigned int);

unsigned int
ll_get_size(linked_list_t *list);

void
ll_free(linked_list_t **pp_list);

void
ll_print_int(linked_list_t *list);

void
ll_print_string(linked_list_t *list);

int
compare_function_ints(void *a, void *b);

int
compare_function_strings(void *a, void *b);

unsigned int
hash_function_int(void *a);

unsigned int
hash_function_string(void *a);

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
		int (*compare_function)(void *, void *));

int
verify_ver(hashtable_t *ht, void *key);

int
ht_has_key(hashtable_t *ht, void *key);

void *
ht_get(hashtable_t *ht, void *key);

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

int
can_I_delete(hashtable_t *ht, void *key);

void
ht_remove_entry(hashtable_t *ht, void *key);

void
ht_free(hashtable_t *ht);

unsigned int
ht_get_size(hashtable_t *ht);

unsigned int
ht_get_hmax(hashtable_t *ht);

ll_node_t *
get_node(linked_list_t *list, unsigned int index);

void
calculate_load_factor_and_decide(hashtable_t **hash);

hashtable_t *
resize_hash(hashtable_t *hash, float load_factor);

char *
allocate_size(char *s);

void
add_book(char *command, char *buffer, hashtable_t *library);

void
remove_basic_newline(char *s);

void
calculate_load_factor_and_decide_for_books(hashtable_t **hash);

hashtable_t *
resize_hash_for_books(hashtable_t *hash, float load_factor);

ll_node_t *
get_book(hashtable_t *library, void *book);

void
remove_book(hashtable_t *library, char *command, char *buffer);

ll_node_t *
get_book_wout_extract(hashtable_t *library, void *book);

void
remove_definition(hashtable_t *library, char *command, char *buffer);

void
get_and_print_def(hashtable_t *library, char *command, char *buffer);

void
add_def_to_book(hashtable_t *library, char *command, char *buffer);

void
add_user_in_system(hashtable_t *users, char *command, char *buffer);

user_features *
init_user_feats(user_features *user);

void
load_factor_decide_for_users(hashtable_t **users);

hashtable_t *
resize_hash_for_users(hashtable_t *users, float load_factor);

void
book_borrow(hashtable_t *library, hashtable_t *users, char *command);

void
borrowing_proccess(hashtable_t *library, hashtable_t *users, char *username,
				char *book_name, unsigned int *days);

void
notify_lost(hashtable_t *library, hashtable_t *users,
		char *command, char *buffer);

void
my_itoa(int n, char *s);

void
my_ftoa(float n, char *s);

void
return_book(hashtable_t *library, hashtable_t *users,
        char *command, char *buffer);

void
get_book_print(hashtable_t *library, char *book_name);

void
library_in_flames(hashtable_t *library, hashtable_t *users);

ll_node_t *
max_book(linked_list_t *buck);

void
resize_hash_at_exit(hashtable_t **hash);


float
get_rating(hashtable_t *book);

ll_node_t *
max_user(linked_list_t *user);


hashtable_t *
resize_hash_for_users_exit(hashtable_t *users, float load_factor);

void
put_atrib_book(book_atrib *book_v, unsigned int bucks_lib,
			hashtable_t *library);

void
put_atrib_user(user_atrib *user_v, unsigned int bucks_users,
			hashtable_t *users);

void
switch_if_the_case(book_atrib *book_v, int i, int j);

#endif  // BIBLE_H_
