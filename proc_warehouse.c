#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

Material create_material();
char* report_warehouse(Warehouse* warehouse, int i);
void add_material(Warehouse* warehouse, Material material);
void print_file(char* filename);
void warehouse_proccess(char* filename, int action, Warehouse* warehouse);
int switch_action();
void add_to_file(char* filename, char* report);
void print_error();

void warehouse_proccess(char* filename, int action, Warehouse* warehouse) {
    while((action = switch_action()) != 3) {
        if (action == 1) {
            print_file(filename);
        } else if (action == 2) {
            Material new_material = create_material();
            add_material(warehouse, new_material);
            for (int i = 0; i < 1; i++) {
                char* string = report_warehouse(warehouse, i);
                add_to_file(filename, string);
            }
        } else {
            print_error();
        }

    }
}

Material create_material() {
    Material new_material;

    printf("Введите наименование материала: ");
    fflush(stdout);
    scanf("%s", new_material.name);

    printf("Введите характеристики материала: ");
    fflush(stdout);
    scanf("%s", new_material.param);

    printf("Введите количество материала: ");
    fflush(stdout);
    scanf("%d", &new_material.quantity);

    return new_material;
}

void add_material(Warehouse *warehouse, Material new_material) {
    if (warehouse->quantity < MAX_AMOUNT_LENGTH) {
        warehouse->materials[warehouse->quantity] = (Material*)malloc(sizeof(Material));
        *(warehouse->materials[warehouse->quantity]) = new_material;
        warehouse->quantity++;
    } else {
        printf("Достигнуто максимально возможное количество материалов\n");
    }
}

char* report_warehouse(Warehouse* warehouse, int i) {
    char* report = (char*)malloc(1000 * sizeof(char));

    if (warehouse->materials[i] != NULL) {
        strcpy(report, "");
        char temp[1000];
        sprintf(temp, "Материал: %s; Свойство: %s; Количество: %d\n", warehouse->materials[i]->name, warehouse->materials[i]->param, warehouse->materials[i]->quantity);
        strncat(report, temp, 1000 - strlen(report) - 1);
    } else {
        strcpy(report, "Invalid material\n");
    }

    return report;
}
