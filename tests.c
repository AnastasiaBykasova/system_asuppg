#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib_main.h"
#include "lib_material.h"
#include "lib_order.h"
#include "lib_staff.h"

int calculate_salary_test(const char *position);
int calculate_salary_manager(const char *position);
int calculate_salary_logist(const char *position);
int calculate_salary_mechanic(const char *position);
int check_data_test(char *user_name, char *password);
int check_data_admin(char *user_name, char *password);
int check_data_storekeeper(char *user_name, char *password);

START_TEST(salary_test_1) {
    int result;
    int expected;
    char *position = "Менеджер";
    result = calculate_salary_test(position);
    expected = calculate_salary_manager(position);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(salary_test_2) {
    int result;
    int expected;
    char *position = "Логист";
    result = calculate_salary_test(position);
    expected = calculate_salary_logist(position);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(salary_test_3) {
    int result;
    int expected;
    char *position = "Механик";
    result = calculate_salary_test(position);
    expected = calculate_salary_mechanic(position);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(salary_test_4) {
    int result;
    int expected;
    char *position = "";
    result = calculate_salary_test(position);
    expected = calculate_salary_mechanic(position);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(auth_test_1) {
    int result;
    int expected;
    char *user_name = "admin";
    char *password = "admin";
    result = check_data_test(user_name, password);
    expected = check_data_admin(user_name, password);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(auth_test_2) {
    int result;
    int expected;
    char *user_name = "storekeeper";
    char *password = "storekeeper";
    result = check_data_test(user_name, password);
    expected = check_data_storekeeper(user_name, password);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(auth_test_3) {
    int result;
    int expected;
    char *user_name = "qwerty";
    char *password = "qwerty";
    result = check_data_test(user_name, password);
    expected = check_data_storekeeper(user_name, password);
    ck_assert_int_eq(result, expected);
}
END_TEST

Suite *asuppg_test(void) {
    Suite *suite = suite_create("asuppg_tests");
    TCase *tcase_core = tcase_create("Core");

    tcase_add_test(tcase_core, salary_test_1);
    tcase_add_test(tcase_core, salary_test_2);
    tcase_add_test(tcase_core, salary_test_3);
    tcase_add_test(tcase_core, salary_test_4);
    tcase_add_test(tcase_core, auth_test_1);
    tcase_add_test(tcase_core, auth_test_2);
    tcase_add_test(tcase_core, auth_test_3);

    suite_add_tcase(suite, tcase_core);

    return suite;
}

// int main(void) {
//     Suite *suite = asuppg_test();
//     SRunner *suite_runner = srunner_create(suite);

//     FILE *test_output = fopen("test_output.xml", "w");
//     if (test_output == NULL) {
//         perror("Error opening file");
//         return 1;
//     }

//     srunner_set_xml(suite_runner, "test_output.xml");  // Устанавливаем XML вывод

//     srunner_run_all(suite_runner, CK_NORMAL);

//     fclose(test_output);  // Закрываем файл

//     // Теперь вывод можно обработать как обычный вывод
//     test_output = fopen("test_output.xml", "r");
//     if (test_output == NULL) {
//         perror("Error opening file");
//         return 1;
//     }
//     char c;
//     while ((c = fgetc(test_output)) != EOF) {
//         printf("%c", c);  // Выводим результаты тестов на экран
//     }
//     fclose(test_output);  // Закрываем файл

//     remove("test_output.xml");  // Удаляем временный файл

//     srunner_free(suite_runner);

//     return 0;

//     // Suite *suite = asuppg_test();
//     // SRunner *suite_runner = srunner_create(suite);
//     // srunner_run_all(suite_runner, CK_NORMAL);

//     // srunner_free(suite_runner);

//     // return 0;
// }

int main(void) {
    Suite *suite = asuppg_test();
    SRunner *suite_runner = srunner_create(suite);
    FILE *test_output = fopen("test_output.xml", "w");
    if (test_output == NULL) {
        perror("Error opening file");
        return 1;
    }
    srunner_set_xml(suite_runner, "test_output.xml");  // Устанавливаем XML вывод
    srunner_run_all(suite_runner, CK_NORMAL);
    fclose(test_output);  // Закрываем файл
    test_output = fopen("test_output.xml", "r");
    if (test_output == NULL) {
        perror("Error opening file");
        return 1;
    }
    char c;
    while ((c = fgetc(test_output)) != EOF) {
        printf("%c", c);  // Выводим результаты тестов на экран
    }
    fclose(test_output);        // Закрываем файл
    remove("test_output.xml");  // Удаляем временный файл
    srunner_free(suite_runner);
    return 0;
}
