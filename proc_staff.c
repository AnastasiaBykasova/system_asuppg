#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

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
    while((action = switch_action()) != 3) {
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
}

Staff create_staff() {
    Staff new_staff;

    printf("Введите имя сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.name);

    printf("Введите статус сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.status);

    printf("Введите дату оформления (mm/dd/yyyy): ");
    fflush(stdout);
    scanf("%s", new_staff.emp_date);

    printf("Введите паспорт сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.passport);

    printf("Введите ИНН сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.inn);
    
    printf("Введите номер телефона сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.phone_num);

    printf("Введите должность сотрудника: ");
    fflush(stdout);
    scanf("%s", new_staff.position);

    new_staff.salary = calculate_salary(new_staff.position);

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
        sprintf(temp, "id: %d; Имя: %s; Статус: %s; Паспорт: %s; Должность: %s; Оклад: %d; Дата оформления: %s; Номер телефона: %s; ИНН: %s\n", staff_headquarters->staff[i]->id, staff_headquarters->staff[i]->name, staff_headquarters->staff[i]->status, staff_headquarters->staff[i]->passport, staff_headquarters->staff[i]->position, staff_headquarters->staff[i]->salary, staff_headquarters->staff[i]->emp_date, staff_headquarters->staff[i]->phone_num, staff_headquarters->staff[i]->inn);
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
