#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "my_lib.h"

int check_data(char* user_name, char* password);
int switch_system_admin();

void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters);
void product_proccess(char* filename, int action, Order* order);
void warehouse_proccess(char* filename, int action, Warehouse* warehouse);
void print_error();

// void the_end(void);

void proccess(char* user_name, char* password) {
    int employee = check_data(user_name, password);
    int subsystem;
    
    if (employee != 0) {
        if (employee == 1) {
            subsystem = switch_system_admin();
        }
        else if (employee == 2) {
            printf("Доступная система: 'Заказы'\n");
            subsystem = 2;
        }
        else {
            printf("Доступная система: 'Склад'\n");
            subsystem = 3;
        }
        
        int action = 0;
        if (subsystem != 0) {
            char filename[MAX_FILENAME_LENGTH];
            Warehouse warehouse;
            StaffHeadquarters staff_headquarters;
            Order order;

            if (subsystem == 1) {
                strcpy(filename, "files/staff.txt");
                staff_proccess(filename, action, &staff_headquarters);

            }
            else if (subsystem == 2) {
                strcpy(filename, "files/orders.txt");
                product_proccess(filename, action, &order);
            }
            else if (subsystem == 3) {
                strcpy(filename, "files/warehouse.txt");
                warehouse_proccess(filename, action, &warehouse);
            }
            else {
                print_error();
            }
        }

        else {
            print_error();
        }
    }
    else {
        print_error();
    }
}

void print_error() {
    printf("Error\n");
}


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
