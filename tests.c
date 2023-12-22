#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib_main.h"
#include "lib_material.h"
#include "lib_order.h"
#include "lib_staff.h"

int calculate_salary(const char *position);
int calculate_salary_test(const char *position);

// S21_EXP_TEST
START_TEST(salary_test) {
    int result;
    int expected;
    char *position = "Менеджер";
    result = calculate_salary(position);
    expected = calculate_salary_test(position);
    ck_assert_int_eq(result, expected);
}
END_TEST

Suite *asuppg_test(void) {
    Suite *suite = suite_create("asuppg_tests");
    TCase *tcase_core = tcase_create("Core");

    tcase_add_test(tcase_core, salary_test);

    suite_add_tcase(suite, tcase_core);

    return suite;
}

int main(void) {
    Suite *suite = asuppg_test();
    SRunner *suite_runner = srunner_create(suite);
    srunner_run_all(suite_runner, CK_NORMAL);

    // Получаем количество проваленных тестов.
    // int failed_count = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return 0;
}
