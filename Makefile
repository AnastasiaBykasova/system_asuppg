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
























# CC = gcc
# # CFLAGS = -Wall -Werror -Wextra
# TARGET = asuppg
# TEST_TARGET = test
# FILES_C = auth.c menu.c common_funcs.c proc*.c test_funcs.c
# FILES_O = auth.o menu.o common_funcs.o proc*.o test_funcs.o
# GTK_FLAGS = $(shell pkg-config --cflags gtk+-3.0)
# GTK_LIB_FLAGS = $(shell pkg-config --libs gtk+-3.0)
# FLAGS_SQL = -lsqlite3 -lpthread 
# TEST_FLAGS = $(shell pkg-config --cflags --libs check)
# LIBS = -L. -l:asuppg


# asuppg: $(FILES_O)
# 	$(CC) $(CFLAGS) -o asuppg main.c $(FILES_O) $(GTK_FLAGS) $(GTK_LIB_FLAGS) $(FLAGS_SQL)

# # test: test_funcs.o proc_staff.o tests.c
# # 	$(CC) -o $(TEST_TARGET) test_funcs.o proc_staff.o tests.c $(TEST_FLAGS)

# test: main.o
# 	$(CC) -o $(TARGET) *.o tests.c $(LIBS) $(TEST_FLAGS)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(FILES_O) $(TARGET) $(TEST_TARGET)

# rebuild: clean asuppg




# CC = gcc
# # CFLAGS = -Wall -Werror -Wextra
# TARGET = asuppg.exe
# FILES_C = auth.c menu.c common_funcs.c proc*.c
# FILES_O = auth.o menu.o common_funcs.o proc*.o
# GTK_FLAGS = $(shell pkg-config --cflags gtk+-3.0)
# GTK_LIB_FLAGS = $(shell pkg-config --libs gtk+-3.0)
# FLAGS_SQL = -lsqlite3 -lpthread 

# all: asuppg.exe

# asuppg.exe: $(FILES_C:.c=.o)
# 	$(CC) -o $(TARGET) main.c $(FILES_O) $(GTK_FLAGS) $(GTK_LIB_FLAGS) $(FLAGS_SQL)

# clean:
# 	rm -f $(FILES_O) $(TARGET)

# rebuild: clean all


# WORKS

# CC = gcc
# CFLAGS = -Wall -Werror -Wextra
# TARGET = asuppg
# FILES_C = auth.c menu.c common_funcs.c proc*.c test_funcs.c
# FILES_O = auth.o menu.o common_funcs.o proc*.o test_funcs.o
# GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
# GTK_LIB_FLAGS = `pkg-config --libs gtk+-3.0`
# FLAGS_SQL = -lsqlite3 -lpthread 
# TEST_FLAGS = `pkg-config --cflags --libs check`


# asuppg:
# 	$(CC) $(CFLAGS) -c $(FILES_C)
# 	$(CC) $(GTK_FLAGS) -o asuppg main.c $(GTK_LIB_FLAGS) $(FILES_O) $(FLAGS_SQL)
# 	rm -f $(FILES_O)

# test:
# 	$(CC) -o $(TARGET) test_funcs.o proc_staff.o tests.c $(TEST_FLAGS)

# clean:
# 	rm -f $(FILES_O) $(TARGET) 

# rebuild: clean asuppg
