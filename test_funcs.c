#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib_main.h"
#include "lib_material.h"
#include "lib_order.h"
#include "lib_staff.h"

int calculate_salary_test(const char* position);

int calculate_salary_test(const char* position) {
    int salary = 0;
    if (strcmp(position, "Менеджер") == 0) {
        salary = 90000;
    } else if (strcmp(position, "Инженер") == 0) {
        salary = 110000;
    } else if (strcmp(position, "Аналитик") == 0) {
        salary = 90000;
    } else if (strcmp(position, "Бухгалтер") == 0) {
        salary = 70000;
    } else if (strcmp(position, "Грузчик") == 0) {
        salary = 50000;
    } else if (strcmp(position, "Комплектовщик") == 0) {
        salary = 60000;
    } else if (strcmp(position, "Логист") == 0) {
        salary = 80000;
    } else if (strcmp(position, "Мастер") == 0) {
        salary = 100000;
    } else if (strcmp(position, "Механик") == 0) {
        salary = 110000;
    } else if (strcmp(position, "Оператор") == 0) {
        salary = 70000;
    } else if (strcmp(position, "Слесарь") == 0) {
        salary = 80000;
    } else if (strcmp(position, "Юрист") == 0) {
        salary = 100000;
    }
    return salary;
}