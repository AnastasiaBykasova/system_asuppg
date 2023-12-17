CC = gcc
CFLAGS = -Wall -Werror -Wextra
TARGET = asuppg
FILES_C = auth.c menu.c common_funcs.c proc*.c
FILES_O = auth.o menu.o common_funcs.o proc*.o
GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIB_FLAGS = `pkg-config --libs gtk+-3.0`
FLAGS_SQL = -lsqlite3 -lpthread 

asuppg:
	$(CC) $(CFLAGS) -c $(FILES_C)
	$(CC) $(GTK_FLAGS) -o asuppg main.c $(GTK_LIB_FLAGS) $(FILES_O) $(FLAGS_SQL)
	rm -f $(FILES_O)
# $(CC) $(CFLAGS) -c $(FILES_C)
# $(CC) $(CFLAGS) -o $(TARGET) main.c $(FILES_O)

# $(CC) $(CFLAGS) -c $(FILES_C)
# $(CC) $(CFLAGS) -o $(TARGET) main.c $(FILES_O) $(pkg-config --cflags --libs gtk+-3.0)
#	gcc -Wall -Werror -Wextra -o asuppg main.c $(pkg-config --cflags --libs gtk+-3.0) auth.o menu.o proc*.o

clean:
	rm -f $(TARGET)

rebuild: clean asuppg
