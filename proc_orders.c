#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib_main.h"
#include "lib_order.h"

void the_end(void);

Product create_product();
char* report_order(Order* order, int i);
void add_product(Order* order, Product new_product);
void print_file(char* filename);
void product_proccess(char* filename, int action, Order* order);
void clear_order(Order* order);
int switch_action();
void add_to_file(char* filename, char* report);
void print_error();
Order create_order();

void product_proccess(char* filename, int action, Order* order) {
    int for_exit = 0;
    for_exit = atexit(the_end);
    while ((action = switch_action()) != 3 && !for_exit) {
        if (action == 1) {
            print_file(filename);
        } else if (action == 2) {
            clear_order(order);
            Product new_product = create_product();
            add_product(order, new_product);
            for (int i = 0; i < order->quantity; i++) {
                char* string = report_order(order, i);
                add_to_file(filename, string);
            }

        } else {
            print_error();
        }
    }
    exit(EXIT_SUCCESS);
}

Product create_product() {
    Product new_product;

    printf("Введите имя заказчика: ");
    fflush(stdout);
    scanf("%s", new_product.customer);

    printf("Введите название товара: ");
    fflush(stdout);
    scanf("%s", new_product.name);

    printf("Введите стоимость товара: ");
    fflush(stdout);
    scanf("%d", &new_product.value);

    printf("Введите количество: ");
    fflush(stdout);
    scanf("%d", &new_product.amount);

    printf("Введите итоговую сумму оплаты: ");
    fflush(stdout);
    scanf("%d", &new_product.sum);

    printf("Введите тип оплаты: ");
    fflush(stdout);
    scanf("%s", new_product.pay_type);

    printf("Введите дату оформления: ");
    fflush(stdout);
    scanf("%s", new_product.date);

    return new_product;
}

void add_product(Order* order, Product new_product) {
    if (order->quantity < MAX_AMOUNT_LENGTH) {
        order->product[order->quantity] = new_product;
        order->quantity++;
    } else {
        printf("Достигнуто максимально возможное количество заказов\n");
    }
}

char* report_order(Order* order, int i) {
    char* report = (char*)malloc(1000 * sizeof(char));

    if (i < order->quantity) {
        strcpy(report, "");
        char temp[1000];
        sprintf(temp,
                "id: %d; Заказчик: %s; товар: %s; стоимость: %d; количество: %d; "
                "сумма: %d; оплата: %s; дата: %s\n",
                order->product[i].id, order->product[i].customer, order->product[i].name,
                order->product[i].value, order->product[i].amount,
                (order->product[i].value) * (order->product[i].amount), order->product[i].pay_type,
                order->product[i].date);
        strncat(report, temp, 1000 - strlen(report) - 1);
    } else {
        strcpy(report, "Invalid product\n");
    }

    return report;
}

void clear_order(Order* order) { order->quantity = 0; }

Order create_order() {
    Order order;
    order.quantity = 0;
    return order;
}

void db_connect_orders() {
    sqlite3* db;
    char* err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        // return 1;
    }
    // char *sql = "DROP TABLE Orders;";

    char* sql =
        "CREATE TABLE IF NOT EXISTS Orders(id INT, Customer TEXT, Name TEXT, "
        "Value INT, Amount INT, Sum INT, PayType TEXT, Date TEXT);"
        "INSERT INTO Orders VALUES(1, 'Арсений1', 'газоблок1', 500, 10, 5000, "
        "'Карта', '14.10.23');"
        "INSERT INTO Orders VALUES(2, 'Арсений2', 'газоблок2', 700, 10, 7000, "
        "'Карта', '15.10.23');"
        "INSERT INTO Orders VALUES(3, 'Арсений3', 'газоблок3', 20, 10, 200, "
        "'Налич', '16.10.23');"
        "INSERT INTO Orders VALUES(4, 'Арсений4', 'газоблок4', 500, 11, 5500, "
        "'Карта', '17.10.23');";

    // char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND
    // name='Orders';";

    // char *sql = "DELETE FROM Staff WHERE id>1;";

    // char *sql = "SELECT * FROM Orders;";

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

void show_order() {
    sqlite3* db;
    // char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char* sql = "SELECT * FROM Orders;";

    sqlite3_stmt* res;
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
        printf("%s|\n", sqlite3_column_text(res, 7));

        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}