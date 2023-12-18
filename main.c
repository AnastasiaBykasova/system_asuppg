#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
#include <gtk/gtk.h>
#include <sqlite3.h>

static GtkWidget *personnel_radio_button;
static GtkWidget *orders_radio_button;
static GtkWidget *warehouse_radio_button;
static GtkWidget *show_radio_button;
static GtkWidget *add_radio_button;
static GtkWidget *delete_radio_button;
static GtkWidget *end_radio_button;
static GtkWidget *grid; // Объявляем grid как глобальную переменную, чтобы иметь к ней доступ из разных функций
GtkWidget *entry_username;
GtkWidget *entry_password;
GtkWidget *auth_button;

int proccess(int employee);
int check_data(char* user_name, char* password);
int switch_system_admin();
void print_error();
void the_end(void);
int calculate_salary( const char* position);
void warehouse_proccess(char* filename, int action, Warehouse* warehouse);
void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters);
void product_proccess(char* filename, int action, Order* order);
static void on_auth_button_clicked(GtkWidget *widget, gpointer data);
static void activate (GtkApplication* app, gpointer user_data);
static void on_orders_button_clicked(GtkWidget *widget, gpointer data);
static void on_warehouse_button_clicked(GtkWidget *widget, gpointer data);
static void choose_subsystem_window(GtkApplication *app, gpointer user_data);
static void on_subsystem_button_clicked(GtkWidget *widget, gpointer user_data);
static void on_action_staff_clicked(GtkWidget *widget, gpointer window, gpointer user_data);
static void on_action_orders_clicked(GtkWidget *widget, gpointer window, gpointer user_data);
static void on_action_materials_clicked(GtkWidget *widget, gpointer window, gpointer user_data);

static void choose_action_window_staff(GtkApplication *app, gpointer user_data);
static void choose_action_window_orders(GtkApplication *app, gpointer user_data);
static void choose_action_window_materials(GtkApplication *app, gpointer user_data);

static void show_staff_in_window(GtkApplication *app, GtkWidget *widget, gpointer user_data);
static void add_staff(GtkApplication *app, gpointer user_data);
static void on_delete_staff_clicked(GtkWidget *widget, gpointer user_data);
static void delete_staff(GtkApplication *app, gpointer user_data);
void db_connect_staff();
void db_connect_orders();
void db_connect_materials();
void show_staff();
void show_order();
void show_material();
int count_id(char* table);
void select_from_staff_table(GtkListStore *store);
static void on_add_staff_clicked(GtkWidget *widget, gpointer user_data);
void add_staff_to_table(int id, const char *name, int passport, int snils, const char *position, int salary, const char *status, const char *emp_date, int phone_num, int inn);
int find_minimum_free_id(int *id_array, int count, int min_id);

static void show_orders_in_window(GtkApplication *app, GtkWidget *widget, gpointer order_data);
void select_from_orders_table(GtkListStore *store);
static void on_add_orders_clicked(GtkWidget *widget, gpointer user_data);
static void add_order(GtkApplication *app, gpointer order_data);
void add_order_to_table(int id, const char *customer,const char *name, int value, int amount, int sum, const char *pay_type, const char *date);
int calculate_sum(int value, int amount);
static void delete_order(GtkApplication *app, gpointer order_data);
static void on_delete_orders_clicked(GtkWidget *widget, gpointer order_data);

static void add_material(GtkApplication *app, gpointer material_data);
static void on_delete_material_clicked(GtkWidget *widget, gpointer user_data);
static void on_add_material_clicked(GtkWidget *widget, gpointer material_data);
void add_material_to_table(int id, const char *name, const char *param, int quantity);
void select_from_materials_table(GtkListStore *store);
static void show_materials_in_window(GtkApplication *app, GtkWidget *widget, gpointer material_data);
static void delete_material(GtkApplication *app, gpointer material_data);

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    int employee;
    GtkWidget *grid;  // добавляем указатель на grid в структуру AppData
} AppData;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *subsystem;
    GtkWidget *grid; 
} SubsystemData;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *name;
    GtkWidget *passport;
    GtkWidget *snils;
    GtkWidget *position;
    // int salary;
    GtkWidget *status;
    GtkWidget *emp_date;
    GtkWidget *phone_num;
    GtkWidget *inn;
    GtkWidget *grid;  // добавляем указатель на grid в структуру StaffAdd
} StaffAdd;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *customer;
    GtkWidget *name;
    GtkWidget *value;
    GtkWidget *amount;
    // GtkWidget *sum;
    GtkWidget *pay_type;
    GtkWidget *date;
    GtkWidget *grid;  // добавляем указатель на grid в структуру StaffAdd
} OrderAdd;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *id;
    GtkWidget *grid;  // добавляем указатель на grid в структуру AppData
} StaffDelete;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *id;
    GtkWidget *grid;  // добавляем указатель на grid в структуру AppData
} OrderDelete;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *name;
    GtkWidget *param;
    GtkWidget *amount;
    GtkWidget *grid;  // добавляем указатель на grid в структуру StaffAdd
} MaterialAdd;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *id;
    GtkWidget *amount;
    GtkWidget *grid;  // добавляем указатель на grid в структуру AppData
} MaterialDelete;

int main(int argc, char **argv) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

    // db_connect_staff();
    // show_staff();
    
    // db_connect_orders();
    // show_order();

    // db_connect_materials();
    // show_material();
    
    return 0;
}

// Обработчик события нажатия кнопки "Войти"
static void on_auth_button_clicked(GtkWidget *widget, gpointer user_data) {
    int for_exit = 0;
    for_exit = atexit(the_end);
    AppData *data = (AppData*)user_data;
    
    if (GTK_IS_ENTRY(data->entry_username) && GTK_IS_ENTRY(data->entry_password)) {
        const gchar *username = gtk_entry_get_text(GTK_ENTRY(data->entry_username));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(data->entry_password));

        int employee = check_data((char*)username, (char*)password);

        if (employee != 0 && !for_exit) {
            // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
            GtkApplication *app;
            app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
            if (employee == 1) {
                g_print("Пользователь: администратор\n");
                g_signal_connect(app, "activate", G_CALLBACK(choose_subsystem_window), NULL);
            }
            else {
                if (employee == 2) {
                    g_print("Пользователь: менеджер по продажам\n");
                    g_print("Доступная подсистема: Заказы\n");
                    g_signal_connect(app, "activate", G_CALLBACK(choose_action_window_orders), NULL);
                }
                else {
                    g_print("Пользователь: сотрудник склада\n");
                    g_print("Доступная подсистема: Склад\n");
                    g_signal_connect(app, "activate", G_CALLBACK(choose_action_window_materials), NULL);

                }
                // g_signal_connect(app, "activate", G_CALLBACK(choose_action_window), NULL);
            }
            g_application_run(G_APPLICATION(app), 0, 0);
            g_object_unref(app);
        } 
        else {
            print_error();
            exit(EXIT_SUCCESS);
        }
    }
    g_slice_free(AppData, data); // Освобождаем память
}

static void on_subsystem_button_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    char* subsystem = NULL;
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(personnel_radio_button))) {
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
        g_print("Выбрана система: Персонал\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(choose_action_window_staff), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
         
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(orders_radio_button))) {
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
        g_print("Выбрана система: Заказы\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(choose_action_window_orders), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
         
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(warehouse_radio_button))) {
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
        g_print("Выбрана система: Склад\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(choose_action_window_materials), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
         
    }
    g_slice_free(AppData, data); // Освобождаем память
}

static void on_add_staff_clicked(GtkWidget *widget, gpointer user_data) {
    StaffAdd *data = (StaffAdd*)user_data;

    const char *name = gtk_entry_get_text(GTK_ENTRY(data->name));
    const char *passport_text = gtk_entry_get_text(GTK_ENTRY(data->passport));
    const char *snils_text = gtk_entry_get_text(GTK_ENTRY(data->snils));
    const char *position = gtk_entry_get_text(GTK_ENTRY(data->position));
    const char *status = gtk_entry_get_text(GTK_ENTRY(data->status));
    const char *emp_date = gtk_entry_get_text(GTK_ENTRY(data->emp_date));
    const char *phone_num_text = gtk_entry_get_text(GTK_ENTRY(data->phone_num));
    const char *inn_text = gtk_entry_get_text(GTK_ENTRY(data->inn));

    // Проверяем, что ни один из полученных указателей не является нулевым или строкой нулевой длины
    if (name && *name != '\0' && passport_text && *passport_text != '\0' && snils_text && *snils_text != '\0' &&
        position && *position != '\0' && status && *status != '\0' && emp_date && *emp_date != '\0' &&
        phone_num_text && *phone_num_text != '\0' && inn_text && *inn_text != '\0') {
        
        // Преобразуем текст в целые числа
        int passport = atoi(passport_text);
        int snils = atoi(snils_text);
        int phone_num = atoi(phone_num_text);
        int inn = atoi(inn_text);
        int salary = calculate_salary(position); // Вычисляем зарплату на основе должности
        // int id = count_id(&id);
        char table[8] = "Staff";
        int id = count_id(table);



        // Вызываем функцию для добавления сотрудника в базу данных
        add_staff_to_table(id, name, passport, snils, position, salary, status, emp_date, phone_num, inn);
    } else {
        // Выводим сообщение об ошибке, если какой-либо из полученных указателей некорректен или данные некорректны
        printf("Ошибка: некорректные данные в полях ввода!\n");
    }

    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    g_slice_free(StaffAdd, data); // Освобождаем память
}

static void on_delete_staff_clicked(GtkWidget *widget, gpointer user_data) {
    StaffDelete *data = (StaffDelete*)user_data;
    sqlite3 *db;
    char *err_msg = 0;

    const char *id_to_delete = gtk_entry_get_text(GTK_ENTRY(data->id));

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        // return 1;
    }

    char *sql = sqlite3_mprintf("DELETE FROM Staff WHERE id=%s;", id_to_delete);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } 
    
    sqlite3_free(sql);
    sqlite3_close(db);


    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    g_slice_free(StaffDelete, data); // Освобождаем память
}


static void on_add_orders_clicked(GtkWidget *widget, gpointer order_data) {
    OrderAdd *data = (OrderAdd*)order_data;

    const char *customer = gtk_entry_get_text(GTK_ENTRY(data->customer));
    const char *name = gtk_entry_get_text(GTK_ENTRY(data->name));
    const char *value_text = gtk_entry_get_text(GTK_ENTRY(data->value));
    const char *amount_text = gtk_entry_get_text(GTK_ENTRY(data->amount));
    const char *pay_type = gtk_entry_get_text(GTK_ENTRY(data->pay_type));
    const char *date = gtk_entry_get_text(GTK_ENTRY(data->date));

    // Проверяем, что ни один из полученных указателей не является нулевым или строкой нулевой длины
    if (customer && *customer != '\0' && name && *name != '\0' && value_text && *value_text != '\0' && 
        amount_text && *amount_text != '\0' && pay_type && *pay_type != '\0' && date && *date != '\0') {
        
        // Преобразуем текст в целые числа
        int value = atoi(value_text);
        int amount = atoi(amount_text);
        int sum = calculate_sum(value, amount); // Вычисляем сумму на основе количества и стоимисти
        char table[8] = "Orders";
        int id = count_id(table);


        // Вызываем функцию для добавления сотрудника в базу данных
        add_order_to_table(id, customer, name, value, amount, sum, pay_type, date);
    } else {
        // Выводим сообщение об ошибке, если какой-либо из полученных указателей некорректен или данные некорректны
        printf("Ошибка: некорректные данные в полях ввода!\n");
    }

    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_orders_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    g_slice_free(OrderAdd, data); // Освобождаем память
}

static void on_delete_orders_clicked(GtkWidget *widget, gpointer order_data) {
    OrderDelete *data = (OrderDelete*)order_data;
    sqlite3 *db;
    char *err_msg = 0;

    const char *id_to_delete = gtk_entry_get_text(GTK_ENTRY(data->id));

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        // return 1;
    }

    char *sql = sqlite3_mprintf("DELETE FROM Orders WHERE id=%s;", id_to_delete);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } 
    
    sqlite3_free(sql);
    sqlite3_close(db);


    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_orders_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    g_slice_free(OrderDelete, data); // Освобождаем память
}


int calculate_sum(int value, int amount) {
    int res = value * amount;
    return res;
}


static void on_add_material_clicked(GtkWidget *widget, gpointer material_data) {
    MaterialAdd *data = (MaterialAdd*)material_data;

    const char *name = gtk_entry_get_text(GTK_ENTRY(data->name));
    const char *param = gtk_entry_get_text(GTK_ENTRY(data->param));
    const char *quantity_text = gtk_entry_get_text(GTK_ENTRY(data->amount));

    // Проверяем, что ни один из полученных указателей не является нулевым или строкой нулевой длины
    if (name && *name != '\0' && param && *param != '\0' && quantity_text && *quantity_text != '\0') {
        
        // Преобразуем текст в целые числа
        int quantity = atoi(quantity_text);
        // int id = count_id(&id);
        char table[10] = "Materials";
        int id = count_id(table);



        // Вызываем функцию для добавления сотрудника в базу данных
        add_material_to_table(id, name, param, quantity);
    } else {
        // Выводим сообщение об ошибке, если какой-либо из полученных указателей некорректен или данные некорректны
        printf("Ошибка: некорректные данные в полях ввода!\n");
    }

    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_materials_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    g_slice_free(MaterialAdd, data); // Освобождаем память
}

static void on_delete_material_clicked(GtkWidget *widget, gpointer material_data) {
    MaterialDelete *data = (MaterialDelete*)material_data;
    sqlite3 *db;
    char *err_msg = 0;

    const char *id_to_delete = gtk_entry_get_text(GTK_ENTRY(data->id));
    const char *amount_to_delete = gtk_entry_get_text(GTK_ENTRY(data->amount));


    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        // return 1;
    }

    char *sql = sqlite3_mprintf("DELETE FROM Materials WHERE id=%s;", id_to_delete);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } 
    
    sqlite3_free(sql);
    sqlite3_close(db);


    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_materials_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    g_slice_free(MaterialDelete, data); // Освобождаем память
}

int count_id(char* table) {
    sqlite3 *db;
    int count = 0;
    int *id_array = NULL;
    int min_id = 301; // Инициализируем min_id значением по умолчанию
    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // const char *sql = "SELECT id FROM Staff;";
    char sql[100];  // Предполагаем, что SQL запрос не превысит 100 символов
    snprintf(sql, sizeof(sql), "SELECT id FROM %s;", table);

    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    int step = sqlite3_step(res);
    while (step == SQLITE_ROW) {
        count += 1; // Увеличиваем количество id
        id_array = realloc(id_array, (count) * sizeof(int)); // Увеличиваем размер массива
        id_array[count - 1] = sqlite3_column_int(res, 0); // Записываем id в массив
        if (id_array[count - 1] < min_id) {
            min_id = id_array[count - 1]; // Находим минимальный id
        }
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    int min_free_id = find_minimum_free_id(id_array, count, min_id);

    free(id_array);
    return min_free_id;
}

int find_minimum_free_id(int *id_array, int count, int min_id) {
    for (int i = 0; (i < count); i++) {
        if (min_id == id_array[i]) {
            min_id++;
        } 
    }
    printf("Присвоен id: %d\n", min_id);
    return min_id;
}

static void on_action_staff_clicked(GtkWidget *widget, gpointer window, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_radio_button))) {
        g_print("Выбран вариант: Показать таблицу\n");

        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(show_orders_in_window), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(show_materials_in_window), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(add_radio_button))) {
        g_print("Выбран вариант: Добавить запись\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(add_staff), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(add_order), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(add_material), NULL);

        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(delete_radio_button))) {
        g_print("Выбран вариант: Удалить запись\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(delete_staff), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(delete_order), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(delete_material), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(end_radio_button))) {
        g_print("Выбран вариант: Завершить работу\n");
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    }
}

static void on_action_orders_clicked(GtkWidget *widget, gpointer window, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_radio_button))) {
        g_print("Выбран вариант: Показать таблицу\n");

        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        // g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
        g_signal_connect(app, "activate", G_CALLBACK(show_orders_in_window), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(show_materials_in_window), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(add_radio_button))) {
        g_print("Выбран вариант: Добавить запись\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        // g_signal_connect(app, "activate", G_CALLBACK(add_staff), NULL);
        g_signal_connect(app, "activate", G_CALLBACK(add_order), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(add_material), NULL);

        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(delete_radio_button))) {
        g_print("Выбран вариант: Удалить запись\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        // g_signal_connect(app, "activate", G_CALLBACK(delete_staff), NULL);
        g_signal_connect(app, "activate", G_CALLBACK(delete_order), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(delete_material), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(end_radio_button))) {
        g_print("Выбран вариант: Завершить работу\n");
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    }
}
static void on_action_materials_clicked(GtkWidget *widget, gpointer window, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_radio_button))) {
        g_print("Выбран вариант: Показать таблицу\n");

        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        // g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(show_orders_in_window), NULL);
        g_signal_connect(app, "activate", G_CALLBACK(show_materials_in_window), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(add_radio_button))) {
        g_print("Выбран вариант: Добавить запись\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        // g_signal_connect(app, "activate", G_CALLBACK(add_staff), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(add_order), NULL);
        g_signal_connect(app, "activate", G_CALLBACK(add_material), NULL);

        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(delete_radio_button))) {
        g_print("Выбран вариант: Удалить запись\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        // g_signal_connect(app, "activate", G_CALLBACK(delete_staff), NULL);
        // g_signal_connect(app, "activate", G_CALLBACK(delete_order), NULL);
        g_signal_connect(app, "activate", G_CALLBACK(delete_material), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(end_radio_button))) {
        g_print("Выбран вариант: Завершить работу\n");
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    }
}


// Обработчики событий нажатия кнопок выбора подсистемы
static void on_orders_button_clicked(GtkWidget *widget, gpointer data) {
    // Логика для выбора подсистемы "Orders"
}

static void on_warehouse_button_clicked(GtkWidget *widget, gpointer data) {
    // Логика для выбора подсистемы "Warehouse"
}

// Функция для создания графического интерфейса и добавления элементов
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    // GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Welcome to AsuPpg!");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Создаем виджеты для аутентификации
    GtkWidget *entry_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_username), "Username");
    gtk_grid_attach(GTK_GRID(grid), entry_username, 0, 0, 1, 1);

    GtkWidget *entry_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password), "Password");
    gtk_grid_attach(GTK_GRID(grid), entry_password, 0, 1, 1, 1);

    GtkWidget *auth_button = gtk_button_new_with_label("Войти");
    gtk_grid_attach(GTK_GRID(grid), auth_button, 0, 2, 1, 1);

    // Создаем виджеты для выбора подсистемы
    subsystem_label = gtk_label_new("Выберите один из вариантов:");
    // Создаем остальные виджеты для выбора подсистемы

    AppData *data = g_slice_new(AppData);
    data->app = app;
    data->entry_username = entry_username;
    data->entry_password = entry_password;
    data->grid = grid;  // Сохраняем grid в структуре AppData
    data->main_window = window;  // Сохраняем указатель на главное окно в структуре AppData

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Войти");
    g_signal_connect(button, "clicked", G_CALLBACK(on_auth_button_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);
    // gtk_box_pack_start(GTK_BOX(v_box), button, FALSE, FALSE, 0);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();

}

static void choose_subsystem_window(GtkApplication *app, gpointer subsystem_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button_next;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Подсистема");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Выберите подсистему для работы:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Добавляем радиокнопки для вариантов ответа
    personnel_radio_button = gtk_radio_button_new_with_label(NULL, "Персонал");
    gtk_grid_attach(GTK_GRID(grid), personnel_radio_button, 0, 1, 1, 1);

    orders_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(personnel_radio_button), "Заказы");
    gtk_grid_attach(GTK_GRID(grid), orders_radio_button, 0, 2, 1, 1);

    warehouse_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(orders_radio_button), "Склад");
    gtk_grid_attach(GTK_GRID(grid), warehouse_radio_button, 0, 3, 1, 1);

    button_next = gtk_button_new_with_label("Далее");
    g_signal_connect(button_next, "clicked", G_CALLBACK(on_subsystem_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_next, 0, 4, 1, 1); // Сохраняем координаты как 0, 4, 1, 1

    gtk_widget_show_all(window);
}

static void choose_action_window_staff(GtkApplication *app, gpointer subsystem_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button_next;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Выбор действия");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Выберите действие:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Добавляем радиокнопки для вариантов ответа
    show_radio_button = gtk_radio_button_new_with_label(NULL, "Показать данные");
    gtk_grid_attach(GTK_GRID(grid), show_radio_button, 0, 1, 1, 1);

    add_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(show_radio_button), "Добавить данные");
    gtk_grid_attach(GTK_GRID(grid), add_radio_button, 0, 2, 1, 1);

    delete_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(add_radio_button), "Удалить данные");
    gtk_grid_attach(GTK_GRID(grid), delete_radio_button, 0, 3, 1, 1);

    end_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(delete_radio_button), "Завершить");
    gtk_grid_attach(GTK_GRID(grid), end_radio_button, 0, 5, 1, 1);

    button_next = gtk_button_new_with_label("Далее");
    g_signal_connect(button_next, "clicked", G_CALLBACK(on_action_staff_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_next, 0, 7, 1, 1); // Сохраняем координаты как 0, 4, 1, 1

    gtk_widget_show_all(window);
}


static void choose_action_window_orders(GtkApplication *app, gpointer subsystem_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button_next;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Выбор действия");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Выберите действие:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Добавляем радиокнопки для вариантов ответа
    show_radio_button = gtk_radio_button_new_with_label(NULL, "Показать данные");
    gtk_grid_attach(GTK_GRID(grid), show_radio_button, 0, 1, 1, 1);

    add_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(show_radio_button), "Добавить данные");
    gtk_grid_attach(GTK_GRID(grid), add_radio_button, 0, 2, 1, 1);

    delete_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(add_radio_button), "Удалить данные");
    gtk_grid_attach(GTK_GRID(grid), delete_radio_button, 0, 3, 1, 1);

    end_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(delete_radio_button), "Завершить");
    gtk_grid_attach(GTK_GRID(grid), end_radio_button, 0, 5, 1, 1);

    button_next = gtk_button_new_with_label("Далее");
    g_signal_connect(button_next, "clicked", G_CALLBACK(on_action_orders_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_next, 0, 7, 1, 1); // Сохраняем координаты как 0, 4, 1, 1

    gtk_widget_show_all(window);
}

static void choose_action_window_materials(GtkApplication *app, gpointer subsystem_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button_next;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Выбор действия");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Выберите действие:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Добавляем радиокнопки для вариантов ответа
    show_radio_button = gtk_radio_button_new_with_label(NULL, "Показать данные");
    gtk_grid_attach(GTK_GRID(grid), show_radio_button, 0, 1, 1, 1);

    add_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(show_radio_button), "Добавить данные");
    gtk_grid_attach(GTK_GRID(grid), add_radio_button, 0, 2, 1, 1);

    delete_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(add_radio_button), "Удалить данные");
    gtk_grid_attach(GTK_GRID(grid), delete_radio_button, 0, 3, 1, 1);

    end_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(delete_radio_button), "Завершить");
    gtk_grid_attach(GTK_GRID(grid), end_radio_button, 0, 5, 1, 1);

    button_next = gtk_button_new_with_label("Далее");
    g_signal_connect(button_next, "clicked", G_CALLBACK(on_action_materials_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_next, 0, 7, 1, 1); // Сохраняем координаты как 0, 4, 1, 1

    gtk_widget_show_all(window);
}

static void show_staff_in_window(GtkApplication *app, GtkWidget *widget, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;

    // Создаем окно
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Персонал");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    // Создаем сетку
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Добавляем метку
    label = gtk_label_new("Таблица данных:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Создаем виджет GtkTreeView
    GtkTreeView *tree_view = GTK_TREE_VIEW(gtk_tree_view_new());
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkListStore *store = gtk_list_store_new(10, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
    
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "id", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "ФИО", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Паспорт", renderer, "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "СНИЛС", renderer, "text", 3, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Должность", renderer, "text", 4, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Оклад", renderer, "text", 5, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Статус", renderer, "text", 6, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Дата оформления", renderer, "text", 7, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Номер телефона", renderer, "text", 8, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "ИНН", renderer, "text", 9, NULL);

    // Заполняем модель данными из базы данных, используя функцию select_from_staff_table()
    select_from_staff_table(store); 

    // Устанавливаем модель для tree_view
    gtk_tree_view_set_model(tree_view, GTK_TREE_MODEL(store));

    // Вставляем виджет tree_view в сетку
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(tree_view), 0, 1, 1, 1);

    // Отображаем окно
    gtk_widget_show_all(window);
}

void select_from_staff_table(GtkListStore *store) {
    sqlite3 *db;

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

    // Заполняем модель store данными из базы данных
    int step = sqlite3_step(res);
    while (step == SQLITE_ROW) {
        // Получаем данные из результата запроса и добавляем их в модель store
        int id = sqlite3_column_int(res, 0);
        const char *name = (const char*)sqlite3_column_text(res, 1);
        int passport = sqlite3_column_int(res, 2);
        int snils = sqlite3_column_int(res, 3);
        const char *position = (const char*)sqlite3_column_text(res, 4);
        int salary = sqlite3_column_int(res, 5);
        const char *status = (const char*)sqlite3_column_text(res, 6);
        const char *emp_date = (const char*)sqlite3_column_text(res, 7);
        int phone_num = sqlite3_column_int(res, 8);
        int inn = sqlite3_column_int(res, 9);
        // printf("\nits id: %d\n", id);

        // Добавляем полученные данные в модель store
        // Добавляем полученные данные в модель store
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, id,
            1, name,  // ФИО
            2, passport,  // Паспорт
            3, snils,  // СНИЛС
            4, position,  // Должность
            5, salary,  // Оклад
            6, status,  // Статус
            7, emp_date,  // Дата оформления
            8, phone_num,  // Номер телефона
            9, inn,  // ИНН
            -1);
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}










static void show_orders_in_window(GtkApplication *app, GtkWidget *widget, gpointer order_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;

    // Создаем окно
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Заказы");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    // Создаем сетку
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Добавляем метку
    label = gtk_label_new("Таблица данных:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Создаем виджет GtkTreeView
    GtkTreeView *tree_view = GTK_TREE_VIEW(gtk_tree_view_new());
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkListStore *store = gtk_list_store_new(8, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);
    
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "id", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Заказчик", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Наименование товара", renderer, "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Стоимость одного товара", renderer, "text", 3, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Количество", renderer, "text", 4, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Сумма", renderer, "text", 5, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Тип оплаты", renderer, "text", 6, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Дата оформления", renderer, "text", 7, NULL);
    

    // Заполняем модель данными из базы данных, используя функцию select_from_staff_table()
    select_from_orders_table(store); 

    // Устанавливаем модель для tree_view
    gtk_tree_view_set_model(tree_view, GTK_TREE_MODEL(store));

    // Вставляем виджет tree_view в сетку
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(tree_view), 0, 1, 1, 1);

    // Отображаем окно
    gtk_widget_show_all(window);
}

void select_from_orders_table(GtkListStore *store) {
    sqlite3 *db;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char *sql = "SELECT * FROM Orders;";

    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Заполняем модель store данными из базы данных
    int step = sqlite3_step(res);
    while (step == SQLITE_ROW) {
        // Получаем данные из результата запроса и добавляем их в модель store
        int id = sqlite3_column_int(res, 0);
        const char *customer = (const char*)sqlite3_column_text(res, 1);
        const char *name = (const char*)sqlite3_column_text(res, 2);
        int value = sqlite3_column_int(res, 3);
        int amount = sqlite3_column_int(res, 4);
        int sum = sqlite3_column_int(res, 5);
        const char *pay_type = (const char*)sqlite3_column_text(res, 6);
        const char *date = (const char*)sqlite3_column_text(res, 7);
        // printf("\nits id: %d\n", id);

        // Добавляем полученные данные в модель store
        // Добавляем полученные данные в модель store
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, id,
            1, customer,
            2, name,  // ФИО
            3, value,  // Паспорт
            4, amount,  // СНИЛС
            5, sum,  // Должность
            6, pay_type,  // Оклад
            7, date,  // Статус
            -1);
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}


static void add_order(GtkApplication *app, gpointer order_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Добавление заказа");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100); 

    // grid = GTK_GRID(gtk_grid_new());
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    // Создаем виджеты для аутентификации
    GtkWidget *order_customer = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(order_customer), "Заказчик");
    gtk_grid_attach(GTK_GRID(grid), order_customer, 0, 0, 1, 1);

    GtkWidget *order_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(order_name), "Наименование товара");
    gtk_grid_attach(GTK_GRID(grid), order_name, 0, 1, 1, 1);

    GtkWidget *order_value = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(order_value), "Стоимость товара");
    gtk_grid_attach(GTK_GRID(grid), order_value, 0, 2, 1, 1);

    GtkWidget *order_amount = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(order_amount), "Количество");
    gtk_grid_attach(GTK_GRID(grid), order_amount, 0, 3, 1, 1);

    GtkWidget *order_pay = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(order_pay), "Тип оплаты");
    gtk_grid_attach(GTK_GRID(grid), order_pay, 0, 5, 1, 1);

    GtkWidget *order_date = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(order_date), "Дата оформления");
    gtk_grid_attach(GTK_GRID(grid), order_date, 0, 6, 1, 1);

    OrderAdd *data = g_slice_new(OrderAdd);
    data->app = app;
    data->customer = order_customer;
    data->name = order_name;
    data->value = order_value;
    data->amount = order_amount;
    data->pay_type = order_pay;
    data->date = order_date;
    data->window = window; // Добавим окно в структуру, чтобы его можно было освободить

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Добавить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_add_orders_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 8, 1, 1);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();
}

static void delete_order(GtkApplication *app, gpointer order_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;
    GtkWidget *label;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Удаление заказа");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100); 

    // grid = GTK_GRID(gtk_grid_new());
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    label = gtk_label_new("Введите id заказа для удаления:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Создаем виджеты для аутентификации
    GtkWidget *id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id), "id");
    gtk_grid_attach(GTK_GRID(grid), id, 0, 1, 1, 1);

    StaffDelete *data = g_slice_new(StaffDelete);
    data->app = app;
    data->id = id;
    data->grid = grid;  // Сохраняем grid в структуре AppData
    data->main_window = window;  // Сохраняем указатель на главное окно в структуре AppData

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Удалить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_delete_orders_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();
}










// Функция для создания графического интерфейса и добавления элементов
static void add_staff(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Добавление сотрудника");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100); 

    // grid = GTK_GRID(gtk_grid_new());
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    // Создаем виджеты для аутентификации
    GtkWidget *staff_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_name), "ФИО");
    gtk_grid_attach(GTK_GRID(grid), staff_name, 0, 0, 1, 1);

    GtkWidget *staff_passport = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_passport), "Паспорт");
    gtk_grid_attach(GTK_GRID(grid), staff_passport, 0, 1, 1, 1);

    GtkWidget *staff_snils = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_snils), "СНИЛС");
    gtk_grid_attach(GTK_GRID(grid), staff_snils, 0, 2, 1, 1);

    GtkWidget *staff_position = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_position), "Должность");
    gtk_grid_attach(GTK_GRID(grid), staff_position, 0, 3, 1, 1);

    GtkWidget *staff_status = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_status), "Статус");
    gtk_grid_attach(GTK_GRID(grid), staff_status, 0, 4, 1, 1);

    GtkWidget *staff_date = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_date), "Дата оформления");
    gtk_grid_attach(GTK_GRID(grid), staff_date, 0, 5, 1, 1);

    GtkWidget *staff_phone = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_phone), "Номер телефона");
    gtk_grid_attach(GTK_GRID(grid), staff_phone, 0, 6, 1, 1);

    GtkWidget *staff_inn = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(staff_inn), "ИНН");
    gtk_grid_attach(GTK_GRID(grid), staff_inn, 0, 7, 1, 1);

    StaffAdd *data = g_slice_new(StaffAdd);
    data->app = app;
    data->name = staff_name;
    data->passport = staff_passport;
    data->snils = staff_snils;
    data->position = staff_position;
    data->status = staff_status;
    data->emp_date = staff_date;
    data->phone_num = staff_phone;
    data->inn = staff_inn;
    data->window = window; // Добавим окно в структуру, чтобы его можно было освободить

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Добавить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_add_staff_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 8, 1, 1);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();
}

static void delete_staff(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;
    GtkWidget *label;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Удаление сотрудника");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100); 

    // grid = GTK_GRID(gtk_grid_new());
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    label = gtk_label_new("Введите id сотрудника для удаления:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Создаем виджеты для аутентификации
    GtkWidget *id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id), "id");
    gtk_grid_attach(GTK_GRID(grid), id, 0, 1, 1, 1);

    StaffDelete *data = g_slice_new(StaffDelete);
    data->app = app;
    data->id = id;
    data->grid = grid;  // Сохраняем grid в структуре AppData
    data->main_window = window;  // Сохраняем указатель на главное окно в структуре AppData

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Удалить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_delete_staff_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();
}

// Добавляет данные персонала в базу данных
void add_staff_to_table(int id, const char *name, int passport, int snils, const char *position, int salary, const char *status, const char *emp_date, int phone_num, int inn) {
    // Создадим экземпляр структуры Staff и заполним его полученными данными
    Staff new_staff;
    new_staff.id = id;
    strncpy(new_staff.name, name, 100);
    new_staff.passport = passport;
    new_staff.snils = snils;
    strncpy(new_staff.position, position, 20);
    new_staff.salary = salary;
    strncpy(new_staff.status, status, 100);
    strncpy(new_staff.emp_date, emp_date, 12);
    new_staff.phone_num = phone_num;
    new_staff.inn = inn;

    // Реализация добавления в базу данных остается практически такой же, как и прежде
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "INSERT INTO Staff VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_int(res, 1, new_staff.id);
    sqlite3_bind_text(res, 2, new_staff.name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 3, new_staff.passport);
    sqlite3_bind_int(res, 4, new_staff.snils);
    sqlite3_bind_text(res, 5, new_staff.position, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 6, new_staff.salary);
    sqlite3_bind_text(res, 7, new_staff.status, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 8, new_staff.emp_date, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 9, new_staff.phone_num);
    sqlite3_bind_int(res, 10, new_staff.inn);

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Сотрудник добавлен успешно\n");
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}













static void show_materials_in_window(GtkApplication *app, GtkWidget *widget, gpointer material_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;

    // Создаем окно
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Сырье");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    // Создаем сетку
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Добавляем метку
    label = gtk_label_new("Таблица данных:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Создаем виджет GtkTreeView
    GtkTreeView *tree_view = GTK_TREE_VIEW(gtk_tree_view_new());
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkListStore *store = gtk_list_store_new(10, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
    
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "id", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Наименование", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Параметры", renderer, "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Количество", renderer, "text", 3, NULL);

    // Заполняем модель данными из базы данных, используя функцию select_from_staff_table()
    select_from_materials_table(store); 

    // Устанавливаем модель для tree_view
    gtk_tree_view_set_model(tree_view, GTK_TREE_MODEL(store));

    // Вставляем виджет tree_view в сетку
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(tree_view), 0, 1, 1, 1);

    // Отображаем окно
    gtk_widget_show_all(window);
}

void select_from_materials_table(GtkListStore *store) {
    sqlite3 *db;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char *sql = "SELECT * FROM Materials;";

    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Заполняем модель store данными из базы данных
    int step = sqlite3_step(res);
    while (step == SQLITE_ROW) {
        // Получаем данные из результата запроса и добавляем их в модель store
        int id = sqlite3_column_int(res, 0);
        const char *name = (const char*)sqlite3_column_text(res, 1);
        const char *param = (const char*)sqlite3_column_text(res, 2);
        int quantity= sqlite3_column_int(res, 3);
        
        // Добавляем полученные данные в модель store
        // Добавляем полученные данные в модель store
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, id,
            1, name,  
            2, param,  
            3, quantity,
            -1);
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}



static void add_material(GtkApplication *app, gpointer material_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Добавление материала");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100); 

    // grid = GTK_GRID(gtk_grid_new());
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    // Создаем виджеты для аутентификации
    GtkWidget *material_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(material_name), "Наименование");
    gtk_grid_attach(GTK_GRID(grid), material_name, 0, 0, 1, 1);

    GtkWidget *material_param = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(material_param), "Параметры");
    gtk_grid_attach(GTK_GRID(grid), material_param, 0, 1, 1, 1);

    GtkWidget *material_quantity = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(material_quantity), "Количество");
    gtk_grid_attach(GTK_GRID(grid), material_quantity, 0, 2, 1, 1);

    MaterialAdd *data = g_slice_new(MaterialAdd);
    data->app = app;
    data->name = material_name;
    data->param = material_param;
    data->amount = material_quantity;
    data->window = window; // Добавим окно в структуру, чтобы его можно было освободить

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Добавить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_add_material_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 8, 1, 1);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();
}

static void delete_material(GtkApplication *app, gpointer material_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;
    GtkWidget *label;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Удаление материала");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100); 

    // grid = GTK_GRID(gtk_grid_new());
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    label = gtk_label_new("Введите id материала для удаления:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Создаем виджеты для аутентификации
    GtkWidget *id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id), "id");
    gtk_grid_attach(GTK_GRID(grid), id, 0, 1, 1, 1);
    
    label = gtk_label_new("Количество материалов для удаления:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    GtkWidget *amount = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(amount), "amount");
    gtk_grid_attach(GTK_GRID(grid), amount, 0, 3, 1, 1);
    

    MaterialDelete *data = g_slice_new(MaterialDelete);
    data->app = app;
    data->id = id;
    data->amount = amount;
    data->grid = grid;  // Сохраняем grid в структуре AppData
    data->main_window = window;  // Сохраняем указатель на главное окно в структуре AppData

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Удалить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_delete_material_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 4, 1, 1);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);

    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();
}

void add_material_to_table(int id, const char *name, const char *param, int quantity) {
    // Создадим экземпляр структуры Staff и заполним его полученными данными
    Material new_material;
    new_material.id = id;
    strncpy(new_material.name, name, 100);
    strncpy(new_material.param, param, 100);
    new_material.quantity = quantity;
    
    // Реализация добавления в базу данных остается практически такой же, как и прежде
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "INSERT INTO Materials VALUES(?, ?, ?, ?)";
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_int(res, 1, new_material.id);
    sqlite3_bind_text(res, 2, new_material.name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 2, new_material.param, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 3, new_material.quantity);

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Материал добавлен успешно\n");
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}



void add_order_to_table(int id, const char *customer,const char *name, int value, int amount, int sum, const char *pay_type, const char *date) {
    // Создадим экземпляр структуры Staff и заполним его полученными данными
    Product new_prod;
    new_prod.id = id;
    strncpy(new_prod.customer, customer, 100);
    strncpy(new_prod.name, name, 100);
    new_prod.value = value;
    new_prod.amount = amount;
    new_prod.sum = sum;
    strncpy(new_prod.pay_type, pay_type, 10);
    strncpy(new_prod.date, date, 12);
    
    // Реализация добавления в базу данных остается практически такой же, как и прежде
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "INSERT INTO Orders VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_int(res, 1, new_prod.id);
    sqlite3_bind_text(res, 2, new_prod.customer, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 3, new_prod.name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 4, new_prod.value);
    sqlite3_bind_int(res, 5, new_prod.amount);
    sqlite3_bind_int(res, 6, new_prod.sum);
    sqlite3_bind_text(res, 7, new_prod.pay_type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 8, new_prod.date, -1, SQLITE_TRANSIENT);

    // Сбросить предыдущую подготовку запроса и связанные с ней данные
    sqlite3_reset(res);
    
    rc = sqlite3_step(res);
    // printf("%d", rc);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL ошибка: %s\n", sqlite3_errmsg(db));
    } 
    else {
        fprintf(stdout, "Заказ добавлен успешно\n");
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}
