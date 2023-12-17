#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
#include <sqlite3.h>

void the_end(void);


Staff create_staff();
char* report_staff_headquarters(StaffHeadquarters* staff_headquarters, int i);
void add_staff(StaffHeadquarters* staff_headquarters, Staff staff);
void print_file(char* filename);
void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters);
void clear_staff_headquarters(StaffHeadquarters* staff_headquarters);
int switch_action();
void add_to_file(char* filename, char* report);
void print_error();
int calculate_salary(char* position);

void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters) {
    int for_exit = 0;
    for_exit = atexit(the_end);
    while((action = switch_action()) != 3 && !for_exit) {
        if (action == 1) {
            print_file(filename);
        } else if (action == 2) {
            clear_staff_headquarters(staff_headquarters);
            Staff new_staff = create_staff();
            add_staff(staff_headquarters, new_staff);
            for (int i = 0; i < staff_headquarters->quantity; i++) {
                char* string = report_staff_headquarters(staff_headquarters, i);
                add_to_file(filename, string);
            }
        } else {
            print_error();
        }
    }
    exit(EXIT_SUCCESS);
}

Staff create_staff() {
    Staff new_staff;

    printf("Введите имя сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.name);

    printf("Введите паспорт сотрудника: ");
    fflush(stdout);
    scanf("%d", &new_staff.passport);

    printf("Введите СНИЛС сотрудника: ");
    fflush(stdout);
    scanf("%d", &new_staff.snils);

    printf("Введите должность сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.position);

    new_staff.salary = calculate_salary(new_staff.position);
    
    printf("Введите статус сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.status);

    printf("Введите дату оформления (mm/dd/yyyy): ");
    fflush(stdout);
    scanf("%s", new_staff.emp_date);

    printf("Введите номер телефона сотрудника: ");
    fflush(stdout);
    scanf("%d", &new_staff.phone_num);
    
    printf("Введите ИНН сотрудника: ");
    fflush(stdout);
    scanf("%d", &new_staff.inn);
    
    return new_staff;
}

void add_staff(StaffHeadquarters *staff_headquarters, Staff new_staff) {
    if (staff_headquarters->quantity < MAX_AMOUNT_LENGTH) {
        staff_headquarters->staff[staff_headquarters->quantity] = (Staff*)malloc(sizeof(Staff));
        *(staff_headquarters->staff[staff_headquarters->quantity]) = new_staff;
        staff_headquarters->quantity++;
    } else {
        printf("Достигнуто максимально возможное количество сотрудников\n");
    }
}

char* report_staff_headquarters(StaffHeadquarters* staff_headquarters, int i) {
    char* report = (char*)malloc(1000 * sizeof(char));

    if (staff_headquarters->staff[i] != NULL) {
        strcpy(report, "");
        char temp[1000];
        sprintf(temp, "Имя: %s; Паспорт: %d; СНИЛС: %d, Должность: %s; Оклад: %d; Статус: %s; Дата оформления: %s; Номер телефона: %d; ИНН: %d\n", staff_headquarters->staff[i]->name, staff_headquarters->staff[i]->passport, staff_headquarters->staff[i]->snils, staff_headquarters->staff[i]->position, staff_headquarters->staff[i]->salary, staff_headquarters->staff[i]->status, staff_headquarters->staff[i]->emp_date, staff_headquarters->staff[i]->phone_num, staff_headquarters->staff[i]->inn);
        strncat(report, temp, 1000 - strlen(report) - 1);
    } else {
        strcpy(report, "Invalid staff\n");
    }

    return report;
}

int calculate_salary(char* position) {
    int salary = 0;
    if (strcmp(position, "Менеджер") == 0) {
        salary = 60000;
    } else if (strcmp(position, "Инженер") == 0) {
        salary = 80000;
    } else if (strcmp(position, "Аналитик") == 0) {
        salary = 70000;
    } else if (strcmp(position, "Бухгалтер") == 0) {
        salary = 65000;
    }
    return salary;
}

void clear_staff_headquarters(StaffHeadquarters* staff_headquarters) {
    for (int i = 0; i < staff_headquarters->quantity; i++) {
        free(staff_headquarters->staff[i]);
    }
    staff_headquarters->quantity = 0;
}


// void the_end(void) {   
//    puts("Работа завершена.");
// }

void db_connect() {
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        // return 1;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS Staff(Name TEXT, Passport INT, SNILS INT, Position TEXT, Salary INT, Status TEXT, Date TEXT, Phone INT, INN INT);"
                "INSERT INTO Staff VALUES('Alice', 1234567890, 11112222333, 'Менеджер', 60000, 'Работает', '25.08.23', '89991234567', 111266677889);";
                // "INSERT INTO Users VALUES(2, 'Bob', 30);"
                // "INSERT INTO Users VALUES(3, 'Charlie', 22);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Table created successfully\n");
        // printf("Hii");
    }

    sqlite3_close(db);

}

void select_from_staff_table() {
    sqlite3 *db;
    // char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char *sql = "SELECT * FROM Staff;";

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
        printf("%s|", sqlite3_column_text(res, 3));
        printf("%s|", sqlite3_column_text(res, 4));
        printf("%s|", sqlite3_column_text(res, 5));
        printf("%s|", sqlite3_column_text(res, 6));
        printf("%s|", sqlite3_column_text(res, 7));
        printf("%s\n", sqlite3_column_text(res, 8));

        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}