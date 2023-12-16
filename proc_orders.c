#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
void the_end(void);

Product create_product();
char* report_order(Order* order, int i);
void add_product(Order *order, Product new_product);
void print_file(char* filename);
void product_proccess(char* filename, int action, Order* order);
void clear_order(Order* order);
int switch_action();
void add_to_file(char* filename, char* report);
void print_error();
Order create_order();

// void track_order(Order* order);
// void update_status(Order* order, char* newStatus);
// void contact_customer(Order* order, char* message);

void product_proccess(char* filename, int action, Order* order) {
    int for_exit = 0;
    for_exit = atexit(the_end);
    while((action = switch_action()) != 3 && !for_exit) {
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

    printf("Введите тип оплаты: ");
    fflush(stdout);
    scanf("%s", new_product.pay_type);

    return new_product;
}

void add_product(Order *order, Product new_product) {
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
        sprintf(temp, "Заказчик: %s; товар: %s; количество: %d; сумма: %d; тип оплаты: %s\n", order->product[i].customer, order->product[i].name, order->product[i].amount, (order->product[i].value)*(order->product[i].amount), order->product[i].pay_type);
        strncat(report, temp, 1000 - strlen(report) - 1);
    } else {
        strcpy(report, "Invalid product\n");
    }

    return report;
}

void clear_order(Order* order) {
    order->quantity = 0;
}

// void track_order(Order* order) {
//     // Логика отслеживания статуса заказа
// }

// void update_status(Order* order, char* newStatus) {
//     strcpy(order->status, newStatus);
// }

// void contact_customer(Order* order, char* message) {
//     // Логика связи с заказчиком
// }

Order create_order() {
    Order order;
    order.quantity = 0;
    return order;
}
