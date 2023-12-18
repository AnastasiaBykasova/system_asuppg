#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
#include <sqlite3.h>

void the_end(void);

Material create_material();
char* report_warehouse(Warehouse* warehouse, int i);
void add_material(Warehouse* warehouse, Material material);
void print_file(char* filename);
void warehouse_proccess(char* filename, int action, Warehouse* warehouse);
int switch_action();
void add_to_file(char* filename, char* report);
void print_error();

void warehouse_proccess(char* filename, int action, Warehouse* warehouse) {
    int for_exit = 0;
    for_exit = atexit(the_end);
    while((action = switch_action()) != 3 && !for_exit) {
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
    exit(EXIT_SUCCESS);
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



void db_connect_materials() {
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        // return 1;
    }
    // char *sql = "DROP TABLE Staff;";
    


    char *sql = "CREATE TABLE IF NOT EXISTS Materials(id INT, Name TEXT, Param TEXT, Quantity INT);"
                "INSERT INTO Materials VALUES(1, 'газоблок1', 'D200', 500);"
                "INSERT INTO Materials VALUES(2, 'газоблок2', 'D300', 350);"
                "INSERT INTO Materials VALUES(3, 'газоблок3', 'D400', 100);"
                "INSERT INTO Materials VALUES(4, 'газоблок4', 'D500', 640);";
    
    // char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='Materials';";
    
    // char *sql = "DELETE FROM Materials WHERE id>0;";
    
    // char *sql = "SELECT * FROM Materials;";


    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "success\n");
        // printf("Hii");
    }

    sqlite3_close(db);

}

void show_material() {
    sqlite3 *db;
    // char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char *sql = "SELECT * FROM Materials;";

    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    int step = sqlite3_step(res);
    while (step == SQLITE_ROW) {
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%s|\n", sqlite3_column_text(res, 3));
        
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}