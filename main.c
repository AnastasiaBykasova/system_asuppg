#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
#include <gtk/gtk.h>


static GtkWidget *personnel_radio_button;
static GtkWidget *orders_radio_button;
static GtkWidget *warehouse_radio_button;
static GtkWidget *grid; // Объявляем grid как глобальную переменную, чтобы иметь к ней доступ из разных функций
// Глобальные переменные для элементов окна выбора подсистемы
GtkWidget *entry_username;
GtkWidget *entry_password;
GtkWidget *auth_button;
GtkWidget *v_box = NULL;
// GtkWidget *window;

int proccess(int employee);
int check_data(char* user_name, char* password);
int switch_system_admin();
void print_error();
void the_end(void);

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

// Объявляем структуру, которая будет хранить данные
// В структуре AppData добавьте указатель на grid
typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *main_window;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
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
}

// Обработчик события нажатия кнопки "Войти"
static void on_auth_button_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    
    if (GTK_IS_ENTRY(data->entry_username) && GTK_IS_ENTRY(data->entry_password)) {
        const gchar *username = gtk_entry_get_text(GTK_ENTRY(data->entry_username));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(data->entry_password));

        int employee = check_data((char*)username, (char*)password);

        if (employee != 0) {
            g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
            GtkApplication *app;
            app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
            g_signal_connect(app, "activate", G_CALLBACK(choose_subsystem_window), NULL);
            g_application_run(G_APPLICATION(app), 0, 0);
            g_object_unref(app);
        } 
    }
    g_slice_free(AppData, data); // Освобождаем память
}

static void on_subsystem_button_clicked(GtkWidget *widget, gpointer window) {
    // AppData *data = (AppData*)user_data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(personnel_radio_button))) {
        g_print("Выбран вариант: Персонал\n");
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(orders_radio_button))) {
        g_print("Выбран вариант: Заказы\n");
        // g_application_quit(G_APPLICATION(data->app));  // Закрываем приложение
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(warehouse_radio_button))) {
        g_print("Выбран вариант: Склад\n");
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
// Функция для создания графического интерфейса и добавления элементов
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    // GtkWidget *grid;
    GtkWidget *subsystem_label; // Добавляем для отображения подсистемы
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Welcome to AsuPpg!");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

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
    gtk_window_set_title(GTK_WINDOW(window), "Welcome to AsuPpg!");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Выберите один из вариантов:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Добавляем радиокнопки для вариантов ответа
    personnel_radio_button = gtk_radio_button_new_with_label(NULL, "Персонал");
    gtk_grid_attach(GTK_GRID(grid), personnel_radio_button, 0, 1, 1, 1);

    orders_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(personnel_radio_button), "Заказы");
    gtk_grid_attach(GTK_GRID(grid), orders_radio_button, 0, 2, 1, 1);

    warehouse_radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(personnel_radio_button), "Склад");
    gtk_grid_attach(GTK_GRID(grid), warehouse_radio_button, 0, 3, 1, 1);

    button_next = gtk_button_new_with_label("Далее");
    g_signal_connect(button_next, "clicked", G_CALLBACK(on_subsystem_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_next, 0, 4, 1, 1); // Сохраняем координаты как 0, 4, 1, 1

    gtk_widget_show_all(window);
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