#include <stdio.h>
#include <string.h>

#include "lib_main.h"
#include "lib_material.h"
#include "lib_order.h"
#include "lib_staff.h"

int check_data(char* user_name, char* password) {
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