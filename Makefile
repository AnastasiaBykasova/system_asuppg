CC = gcc
CFLAGS = -Wall -Werror -Wextra
TARGET = asuppg
TEST_TARGET = test
FILES_C = auth.c menu.c common_funcs.c proc*.c test_funcs.c
FILES_O = auth.o menu.o common_funcs.o proc*.o test_funcs.o
GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIB_FLAGS = `pkg-config --libs gtk+-3.0`
FLAGS_SQL = -lsqlite3 -lpthread 
TEST_FLAGS = `pkg-config --cflags --libs check`


asuppg:
	$(CC) $(CFLAGS) -c $(FILES_C)
	$(CC) $(GTK_FLAGS) -o asuppg main.c $(GTK_LIB_FLAGS) $(FILES_O) $(FLAGS_SQL)
	rm -f $(FILES_O)

test:
	$(CC) $(CFLAGS) -c $(FILES_C)
	$(CC) $(GTK_FLAGS) -o asuppg main.c $(GTK_LIB_FLAGS) $(FILES_O) $(FLAGS_SQL)
	$(CC) -o $(TEST_TARGET) test_funcs.o tests.c $(TEST_FLAGS)
	rm -f $(FILES_O)


clean:
	rm -f $(FILES_O) $(TARGET) $(TEST_TARGET) 

rebuild: clean asuppg test 

