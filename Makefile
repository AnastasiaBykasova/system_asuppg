CC = gcc
CFLAGS = -Wall -Werror -Wextra
TARGET = asuppg
FILES_C = auth.c menu.c proc*.c
FILES_O = auth.o menu.o proc*.o

asuppg:
	$(CC) $(CFLAGS) -c $(FILES_C)
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(FILES_O)

clean:
	rm -f $(TARGET) $(FILES_O)

rebuild: clean asuppg
