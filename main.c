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
static GtkWidget *end_radio_button;
static GtkWidget *grid; // Объявляем grid как глобальную переменную, чтобы иметь к ней доступ из разных функций
// Глобальные переменные для элементов окна выбора подсистемы
GtkWidget *entry_username;
GtkWidget *entry_password;
GtkWidget *auth_button;
// GtkWidget *v_box = NULL;
// GtkWidget *window;

int proccess(int employee);
int check_data(char* user_name, char* password);
int switch_system_admin();
void print_error();
void the_end(void);
// int calculate_salary(char* position);
int calculate_salary( const char* position);

void warehouse_proccess(char* filename, int action, Warehouse* warehouse);
void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters);
void product_proccess(char* filename, int action, Order* order);
static void on_auth_button_clicked(GtkWidget *widget, gpointer data);
static void activate (GtkApplication* app, gpointer user_data);
static void on_orders_button_clicked(GtkWidget *widget, gpointer data);
// static void on_subsystem_button_clicked(GtkWidget *widget, GtkApplication* app);
static void on_warehouse_button_clicked(GtkWidget *widget, gpointer data);

// static void choose_subsystem_window(GtkApplication *app, gpointer user_data, int employee);
static void choose_subsystem_window(GtkApplication *app, gpointer user_data);
static void on_subsystem_button_clicked(GtkWidget *widget, gpointer user_data);
static void on_action_button_clicked(GtkWidget *widget, gpointer window, gpointer user_data);
static void choose_action_window(GtkApplication *app, gpointer user_data);
static void show_staff_in_window(GtkApplication *app, GtkWidget *widget, gpointer user_data);
static void add_staff(GtkApplication *app, gpointer user_data);

// void db_connect();
// void show_staff();

void select_from_staff_table(GtkListStore *store);
static void on_add_staff_clicked(GtkWidget *widget, gpointer user_data);
// void staff_add_to_table(GtkWidget *button, Staff *data);
// void add_staff_to_table(char *name, const char *passport, const char *snils, char *position, const int salary, const char *status, const char *emp_date, const int phone_num, const int inn);
// void add_staff_to_table(char *name, int passport, int snils, char *position, int salary, char *status, char *emp_date, int phone_num, int inn);
// void add_staff_to_table(char *name, const char *passport, const char *snils, char *position, const int salary, const char *status, const char *emp_date, const int phone_num, const int inn);
// void add_staff_to_table(char *name, const char *passport, const char *snils, char *position, const int salary, const char *status, const char *emp_date, const int phone_num, const int inn);
void add_staff_to_table(char *name, int passport, int snils, char *position, int salary, char *status, char *emp_date, int phone_num, int inn);


// void add_staff_to_table(const char *name, const char *passport, const char *snils, const char *position, const char *status, const char *emp_date, const char *phone_num, const char *inn);
// void add_staff_to_table(char *name, int passport, int snils, char *position, int salary, char *status, char *emp_date, int phone_num, int inn);
// Объявляем структуру, которая будет хранить данные
// В структуре AppData добавьте указатель на grid
typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    int employee;
    GtkWidget *grid;  // добавляем указатель на grid в структуру AppData
} AppData;


int main(int argc, char **argv) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

    // db_connect();
    // show_staff();
    // return 0;
}

// Обработчик события нажатия кнопки "Войти"
static void on_auth_button_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    
    if (GTK_IS_ENTRY(data->entry_username) && GTK_IS_ENTRY(data->entry_password)) {
        const gchar *username = gtk_entry_get_text(GTK_ENTRY(data->entry_username));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(data->entry_password));

        int employee = check_data((char*)username, (char*)password);

        if (employee != 0) {
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
                }
                else {
                    g_print("Пользователь: сотрудник склада\n");
                    g_print("Доступная подсистема: Склад\n");
                }
                g_signal_connect(app, "activate", G_CALLBACK(choose_action_window), NULL);
            }
            g_application_run(G_APPLICATION(app), 0, 0);
            g_object_unref(app);
        } 
    }
    g_slice_free(AppData, data); // Освобождаем память
}

static void on_subsystem_button_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    char* subsystem = NULL;
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(personnel_radio_button)) || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(orders_radio_button)) || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(warehouse_radio_button))) {
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(choose_action_window), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
         
    }
    g_slice_free(AppData, data); // Освобождаем память
}

static void on_add_staff_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    
    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
    
    g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);
        
    
    g_slice_free(AppData, data); // Освобождаем память
}


static void on_action_button_clicked(GtkWidget *widget, gpointer window, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_radio_button))) {
        // g_print("Выбран вариант: Показать\n");

        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(show_staff_in_window), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);


        
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(add_radio_button))) {
        // g_print("Выбран вариант: Добавить\n");
        GtkApplication *app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        
        g_signal_connect(app, "activate", G_CALLBACK(add_staff), NULL);
        
        g_application_run(G_APPLICATION(app), 0, 0);
        g_object_unref(app);
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(end_radio_button))) {
        g_print("Выбран вариант: Завершить\n");
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    }
    // g_object_unref(app);
    // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    // g_slice_free(AppData, data); // Освобождаем память
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




static void choose_subsystem_window(GtkApplication *app, gpointer user_data) {
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

static void choose_action_window(GtkApplication *app, gpointer user_data) {
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

    end_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(add_radio_button), "Завершить");
    gtk_grid_attach(GTK_GRID(grid), end_radio_button, 0, 3, 1, 1);

    button_next = gtk_button_new_with_label("Далее");
    g_signal_connect(button_next, "clicked", G_CALLBACK(on_action_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_next, 0, 4, 1, 1); // Сохраняем координаты как 0, 4, 1, 1

    gtk_widget_show_all(window);
}


static void show_staff_in_window(GtkApplication *app, GtkWidget *widget, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    

    // Создаем окно
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Персонал");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
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
    GtkListStore *store = gtk_list_store_new(9, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);

    // Добавляем столбцы к модели store с помощью gtk_tree_view_insert_column_with_attributes()
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "ФИО", renderer, "text", 0, NULL);
    // gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Паспорт", renderer, "int", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Паспорт", renderer, "text", 1, NULL);
    // gtk_tree_view_insert_column_with_attributes(tree_view, -1, "СНИЛС", renderer, "int", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "СНИЛС", renderer, "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Должность", renderer, "text", 3, NULL);
    // gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Оклад", renderer, "int", 4, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Оклад", renderer, "text", 4, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Статус", renderer, "text", 5, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Дата оформления", renderer, "text", 6, NULL);
    // gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Номер телефона", renderer, "int", 7, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Номер телефона", renderer, "text", 7, NULL);
    // gtk_tree_view_insert_column_with_attributes(tree_view, -1, "ИНН", renderer, "int", 8, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "ИНН", renderer, "text", 8, NULL);


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
        const char *name = (const char*)sqlite3_column_text(res, 0);
        int passport = sqlite3_column_int(res, 1);
        int snils = sqlite3_column_int(res, 2);
        const char *position = (const char*)sqlite3_column_text(res, 3);
        int salary = sqlite3_column_int(res, 4);
        const char *status = (const char*)sqlite3_column_text(res, 5);
        const char *emp_date = (const char*)sqlite3_column_text(res, 6);
        int phone_num = sqlite3_column_int(res, 7);
        int inn = sqlite3_column_int(res, 8);

        // Добавляем полученные данные в модель store
        // Добавляем полученные данные в модель store
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, name,  // ФИО
            1, passport,  // Паспорт
            2, snils,  // СНИЛС
            3, position,  // Должность
            4, salary,  // Оклад
            5, status,  // Статус
            6, emp_date,  // Дата оформления
            7, phone_num,  // Номер телефона
            8, inn,  // ИНН
            -1);
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}


// Функция для создания графического интерфейса и добавления элементов
static void add_staff(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    // GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Добавление персонала");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_move(GTK_WINDOW(window), 100, 100);  // Новые координаты окна (x, y)
    // gtk_window_fullscreen(GTK_WINDOW(window));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

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

    // GtkWidget *add_staff_button = gtk_button_new_with_label("Добавить");
    // gtk_grid_attach(GTK_GRID(grid), add_staff_button, 0, 8, 1, 1);

    // Создаем виджеты для выбора подсистемы
    // subsystem_label = gtk_label_new("Выберите один из вариантов:");
    // Создаем остальные виджеты для выбора подсистемы

    AppData *data = g_slice_new(AppData);
    data->app = app;
    data->entry_username = entry_username;
    data->entry_password = entry_password;
    data->grid = grid;  // Сохраняем grid в структуре AppData
    data->main_window = window;  // Сохраняем указатель на главное окно в структуре AppData

    // Запоминаем grid
    grid = grid;

    button = gtk_button_new_with_label("Добавить");
    g_signal_connect(button, "clicked", G_CALLBACK(on_add_staff_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 8, 1, 1);
    // gtk_box_pack_start(GTK_BOX(v_box), button, FALSE, FALSE, 0);
   
    // Удаляем виджет auth_button из grid
    // gtk_container_remove(GTK_CONTAINER(grid), auth_button);


    gtk_widget_show_all(window);
    // g_signal_connect(window, "destroy", G_CALLBACK(choose_subsystem_window), NULL);
    // gtk_main();

}


// Добавляет данные персонала в базу данных
void add_staff_to_table(char *name, int passport, int snils, char *position, int salary, char *status, char *emp_date, int phone_num, int inn) {
    // Создадим экземпляр структуры Staff и заполним его полученными данными
    Staff new_staff;
    
    strncpy(new_staff.name, name, 100);
    new_staff.passport = passport;
    new_staff.snils = snils;
    strncpy(new_staff.position, position, 20);
    new_staff.salary = salary;
    strncpy(new_staff.status, status, 100);
    strncpy(new_staff.emp_date, emp_date, 12);
    new_staff.phone_num = phone_num;
    new_staff.inn = inn;

    // Теперь, когда у нас есть структура Staff с данными, мы можем их сохранить в базе данных
    // Реализация добавления в базу данных остается практически такой же, как и прежде

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("asuppg.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "INSERT INTO Staff VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, sql, -1, &res, 0);

    sqlite3_bind_text(res, 1, new_staff.name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 2, new_staff.passport);
    sqlite3_bind_int(res, 3, new_staff.snils);
    sqlite3_bind_text(res, 4, new_staff.position, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 5, new_staff.salary);
    sqlite3_bind_text(res, 6, new_staff.status, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 7, new_staff.emp_date, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(res, 8, new_staff.phone_num);
    sqlite3_bind_int(res, 9, new_staff.inn);

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Staff added successfully\n");
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}






int proccess(int employee) {
    // int employee = check_data(user_name, password);
    int subsystem;
    int for_exit = 0;
    for_exit = atexit(the_end);
    int res = 0;
    
    if (employee != 0 && !for_exit) {
        if (employee == 1) {
            subsystem = switch_system_admin();
        }
        else if (employee == 2) {
            printf("Доступная система: 'Заказы'\n");
            subsystem = 2;
        }
        else {
            printf("Доступная система: 'Склад'\n");
            subsystem = 3;
        }
        
        int action = 0;
        if (subsystem != 0) {
            char filename[MAX_FILENAME_LENGTH];
            Warehouse warehouse;
            StaffHeadquarters staff_headquarters;
            Order order;

            if (subsystem == 1) {
                strcpy(filename, "files/staff.txt");
                staff_proccess(filename, action, &staff_headquarters);
                res = 1;

            }
            else if (subsystem == 2) {
                strcpy(filename, "files/orders.txt");
                product_proccess(filename, action, &order);
                res = 1;
            }
            else if (subsystem == 3) {
                strcpy(filename, "files/warehouse.txt");
                warehouse_proccess(filename, action, &warehouse);
                res = 1;
            }
            else {
                print_error();
            }
        }

        else {
            print_error();
        }
    }
    else {
        print_error();
        exit(EXIT_SUCCESS);
    }
    return res;
}

void print_error() {
    printf("Error\n");
}

void print_file(char* filename) {
    printf("Содержимое файла %s:\n", filename);
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        char buffer[1000];
        while (fgets(buffer, 1000, file) != NULL) {
            printf("%s", buffer);
        }
        fclose(file);
    } else {
        printf("Ошибка открытия файла\n");
    }
}

void add_to_file(char* filename, char* report) {
    FILE *file = fopen(filename, "a+");
    if (file != NULL) {
        fputs(report, file);
        fclose(file);
    }
    free(report);
}

