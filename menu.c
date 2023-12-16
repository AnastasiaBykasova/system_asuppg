#include <stdio.h>
#include <string.h>
#include "my_lib.h"

int switch_system_admin() {
    int choice;
    printf("\nВыберите подсистему для работы:\n1. Персонал\n2. Заказы\n3. Склад\n");
    printf("Введите номер подсистемы: ");
    scanf("%d", &choice);
    int res = 0;

    switch(choice) {
        case 1:
            res = 1;
            break;
        case 2:
            res = 2;
            break;
        case 3:
            res = 3;
            break;
        default:
            res = 0;
            break;
    }
    return res;
}



int switch_action() {
    int choice;
    printf("\nВыберите действие:\n1. Показать данные\n2. Добавить данные\n3. Завершить\n");
    printf("Введите номер действия: ");
    scanf("%d", &choice);
    int res;

    switch(choice) {
        case 1:
            res = 1;
            break;
        case 2:
            res = 2;
            break;
        case 3:
            res = 3;
            break;
        default:
            res = 0;
            break;
    }
    return res;
}
