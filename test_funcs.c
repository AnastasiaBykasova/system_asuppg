#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int calculate_salary_manager(const char* position) {
    int salary;
    if (strcmp(position, "Менеджер") == 0) {
        salary = 90000;
    } else {
        salary = 0;
    }
    return salary;
}

int calculate_salary_logist(const char* position) {
    int salary;
    if (strcmp(position, "Логист") == 0) {
        salary = 80000;
    } else {
        salary = 0;
    }
    return salary;
}

int calculate_salary_mechanic(const char* position) {
    int salary;
    if (strcmp(position, "Механик") == 0) {
        salary = 110000;
    } else {
        salary = 0;
    }
    return salary;
}

int check_data_test(char* user_name, char* password) {
    int res = 0;
    if (strcmp(user_name, "admin") == 0 && strcmp(password, "admin") == 0) {
        res = 1;
    } else if (strcmp(user_name, "manager") == 0 && strcmp(password, "manager") == 0) {
        res = 2;
    } else if (strcmp(user_name, "storekeeper") == 0 && strcmp(password, "storekeeper") == 0) {
        res = 3;
    }
    return res;
}

int check_data_admin(char* user_name, char* password) {
    int res = 0;
    if (strcmp(user_name, "admin") == 0 && strcmp(password, "admin") == 0) {
        res = 1;
    }
    return res;
}

int check_data_storekeeper(char* user_name, char* password) {
    int res = 0;
    if (strcmp(user_name, "storekeeper") == 0 && strcmp(password, "storekeeper") == 0) {
        res = 3;
    }
    return res;
}