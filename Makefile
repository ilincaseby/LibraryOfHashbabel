CFLAGS = -Wall -Wextra -std=c99

build: main

main: main.o remove_books_defs.o resize.o user_add.o lost.o list_implem.o hash_implem.o get_book.o get_add_defs.o exit_please.o command_prelucration.o borrow_and_return.o adding_books.o
		$(CC) $(CFLAGS) -o $@ $^

main.o: main.c

remove_books_defs.o: remove_books_defs.c

resize.o: resize.c

user_add.o: user_add.c

lost.o: lost.c

list_implem.o: list_implem.c

hash_implem.o: hash_implem.c

get_book.o: get_book.c

get_add_defs.o: get_add_defs.c

exit_please.o: exit_please.c

command_prelucration.o: command_prelucration.c

borrow_and_return.o: borrow_and_return.c

adding_books.o: adding_books.c

clean:
		-rm *.o main