#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
#include <gtk/gtk.h>

void proccess(char* user_name, char* password);
int check_data(char* user_name, char* password);
int switch_system_admin();
void print_error();
void warehouse_proccess(char* filename, int action, Warehouse* warehouse);
void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters);
void product_proccess(char* filename, int action, Order* order);
static void on_button_clicked(GtkWidget *widget, gpointer data);
static void activate (GtkApplication* app, gpointer user_data);
static void on_orders_button_clicked(GtkWidget *widget, gpointer data);
static void open_subsystem_selection_window(GtkWidget *widget, GtkApplication* app);
static void on_warehouse_button_clicked(GtkWidget *widget, gpointer data);

// Объявляем структуру, которая будет хранить данные
typedef struct {
    GtkApplication *app;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
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
static void on_button_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(data->entry_username));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(data->entry_password));
    proccess((char*)username, (char*)password);

    g_slice_free(AppData, data); // Освобождаем память
}


// Функция для создания окна выбора подсистемы
static void open_subsystem_selection_window(GtkWidget *widget, GtkApplication* app) {
    GtkWidget *subsystem_window;
    GtkWidget *grid;
    GtkWidget *button_orders;
    GtkWidget *button_warehouse;

    subsystem_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(subsystem_window), "Select Subsystem");
    gtk_window_set_default_size(GTK_WINDOW(subsystem_window), 800, 800);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(subsystem_window), grid);

    app = GTK_APPLICATION(gtk_widget_get_ancestor(widget, GTK_TYPE_APPLICATION)); // Changed this line to get the application

    gtk_grid_attach(GTK_GRID(grid), button_orders, 0, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), button_warehouse, 0, 1, 1, 1);
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
    GtkWidget *grid;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Welcome to AsuPpg!");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_username), "Username");
    gtk_grid_attach(GTK_GRID(grid), entry_username, 0, 0, 1, 1);

    // Поле для ввода пароля
    entry_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE); // Отключаем отображение введенных символов
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password), "Password");
    gtk_grid_attach(GTK_GRID(grid), entry_password, 0, 1, 1, 1);

    button = gtk_button_new_with_label("Войти");
    g_signal_connect(button, "clicked", G_CALLBACK(open_subsystem_selection_window), window); // Изменение обработчика на открытие меню
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    gtk_widget_show_all(window);


    AppData *data = g_slice_new(AppData);
    data->app = app;
    data->entry_username = entry_username;
    data->entry_password = entry_password;

    button = gtk_button_new_with_label("Войти");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    gtk_widget_show_all(window);
}

void proccess(char* user_name, char* password) {
    int employee = check_data(user_name, password);
    int subsystem;
    
    if (employee != 0) {
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

            }
            else if (subsystem == 2) {
                strcpy(filename, "files/orders.txt");
                product_proccess(filename, action, &order);
            }
            else if (subsystem == 3) {
                strcpy(filename, "files/warehouse.txt");
                warehouse_proccess(filename, action, &warehouse);
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
    }
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

