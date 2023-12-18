#include <stdio.h>
#include <stdlib.h>

#include "my_lib.h"
// void the_end(void);

void the_end(void) { puts("Работа завершена."); }

void print_file(char* filename) {
    printf("Содержимое файла %s:\n", filename);
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        char buffer[1000];
        while (fgets(buffer, 1000, file) != NULL) {
            printf("%s", buffer);
        }
        fclose(file);
    } else {
        printf("Ошибка открытия файла\n");
    }
}

void add_to_file(char* filename, char* report) {
    FILE* file = fopen(filename, "a+");
    if (file != NULL) {
        fputs(report, file);
        fclose(file);
    }
    free(report);
}

void print_error() { printf("Error\n"); }
