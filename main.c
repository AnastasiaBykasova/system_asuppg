#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h"
#include <gtk/gtk.h>


// void proccess(char* user_name, char* password);
void proccess(char* user_name, char* password, GtkApplication *app);
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
void update_staff_list_view(GtkWidget *list_view, GtkListStore *store, StaffHeadquarters *staff_headquarters);
void add_staff_to_list_store(GtkListStore *store, const Staff *staff);
Staff create_staff_dialog(GtkWidget *parent);
// Измененный прототип функции staff_proccess_dialog
// void staff_proccess_dialog(GtkWidget *parent, const char *filename, int action, StaffHeadquarters *staff_headquarters);
// void staff_proccess_dialog(GtkWidget *parent, char *filename, int action, StaffHeadquarters *staff_headquarters);
// Измененный прототип функции staff_proccess_dialog
void staff_proccess_dialog(GtkWidget *parent, char *filename, int action, StaffHeadquarters *staff_headquarters);

// void staff_proccess_dialog(GtkWidget *parent, char *filename, int action, StaffHeadquarters *staff_headquarters);
// static void create_staff_button_clicked(GtkWidget *widget, gpointer data);
void the_end(void);
void print_file_dialog(GtkWidget *parent, const gchar *filename);
void clear_staff_headquarters(StaffHeadquarters* staff_headquarters);
char* report_staff_headquarters(StaffHeadquarters* staff_headquarters, int i);
void print_error_dialog(GtkWidget *parent, const gchar *message);
int switch_system_admin_dialog(GtkApplication *app);
int calculate_salary(char* position);
void print_available_system_dialog(GtkApplication *app, const gchar *system_name);
int switch_action();



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

// Обновленный обработчик события нажатия кнопки "Войти"
static void on_button_clicked(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(data->entry_username));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(data->entry_password));
    proccess((char*)username, (char*)password, data->app);

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

void proccess(char* user_name, char* password, GtkApplication *app) {
    int employee = check_data(user_name, password);
    int subsystem;

    if (employee != 0) {
        if (employee == 1) {
            subsystem = switch_system_admin_dialog(app);
        }
        else if (employee == 2) {
            print_available_system_dialog(app, "Orders");
            subsystem = 2;
        }
        else {
            print_available_system_dialog(app, "Warehouse");
            subsystem = 3;
        }
        
        int action = 0;
        if (subsystem != 0) {
            const gchar *filename;
            Warehouse warehouse;
            StaffHeadquarters staff_headquarters;
            Order order;

            if (subsystem == 1) {
                filename = "files/staff.txt";
                staff_proccess_dialog(GTK_WIDGET(app), filename, action, &staff_headquarters);
            }
            // else if (subsystem == 2) {
            //     filename = "files/orders.txt";
            //     product_proccess_dialog(app, filename, action, &order);
            // }
            // else if (subsystem == 3) {
            //     filename = "files/warehouse.txt";
            //     warehouse_proccess_dialog(app, filename, action, &warehouse);
            // }
            else {
                print_error_dialog(GTK_WIDGET(gtk_application_get_active_window(app)), "Error");
            }
        }

        else {
            print_error_dialog(GTK_WIDGET(gtk_application_get_active_window(app)), "Error");
        }
    }
    else {
        print_error_dialog(GTK_WIDGET(gtk_application_get_active_window(app)), "Error");
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



// Функция создания нового сотрудника с использованием диалоговых окон GTK+
Staff create_staff_dialog(GtkWidget *parent) {
    Staff new_staff;

    // Создаем диалоговые окна для ввода информации о сотруднике
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Create Staff", GTK_WINDOW(parent), GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Добавляем поля для ввода информации
    // Например, для имени:
    GtkWidget *label = gtk_label_new("Имя сотрудника:");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    GtkWidget *entry_name = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), entry_name);

    // Аналогично добавляем другие поля для ввода информации

    // Отображаем диалоговое окно и ждем ввода от пользователя
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK) {
        // Получаем введенные значения
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
        // Аналогично для других полей

        // Заполняем поля нового сотрудника
        strcpy(new_staff.name, name);
        // Аналогично для других полей
        // Вычисляем оклад на основе должности
        new_staff.salary = calculate_salary(new_staff.position);
    }
    gtk_widget_destroy(dialog); // Уничтожаем диалоговое окно

    return new_staff;
}

void add_staff_to_list_store(GtkListStore *store, const Staff *staff) {
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, COLUMN_ID, staff->id, COLUMN_NAME, staff->name, COLUMN_STATUS, staff->status, COLUMN_POSITION, staff->position, COLUMN_SALARY, staff->salary, -1);
}

// Функция для обновления списка сотрудников в интерфейсе
void update_staff_list_view(GtkWidget *list_view, GtkListStore *store, StaffHeadquarters *staff_headquarters) {
    // Очищаем список
    gtk_list_store_clear(store);

    // Добавляем новых сотрудников в список
    for (int i = 0; i < staff_headquarters->quantity; i++) {
        add_staff_to_list_store(store, staff_headquarters->staff[i]);
    }
}

// Функция для обработки события создания нового сотрудника
// Обновленная функция create_staff_button_clicked
// static void create_staff_button_clicked(GtkWidget *widget, gpointer data) {
//     GtkWidget *parent = GTK_WIDGET(data);
//     Staff new_staff = create_staff_dialog(parent);
//     add_staff_to_list_store(store, &new_staff); // Используем add_staff_to_list_store вместо add_staff
//     update_staff_list_view(list_view, store, staff_headquarters);
// }
// Предположим, что store и list_view уже объявлены и инициализированы

// Исправленная функция create_staff_button_clicked
static void create_staff_button_clicked(GtkWidget *widget, gpointer data) {
    GtkListStore *store;
    GtkTreeView *list_view;
    GtkWidget *parent = GTK_WIDGET(data);
    Staff new_staff = create_staff_dialog(parent);
    add_staff_to_list_store(store, &new_staff); // Используем add_staff_to_list_store вместо add_staff
    update_staff_list_view(list_view, store, &staff_headquarters); // Предполагая, что staff_headquarters представляет структуру/объект StaffHeadquarters
}



// Обновленная функция staff_proccess, использование GTK+
void staff_proccess_dialog(GtkWidget *parent, char *filename, int action, StaffHeadquarters *staff_headquarters) {
    int for_exit = 0;
    for_exit = atexit(the_end);

    while ((action = switch_action()) != 3 && !for_exit) {
        if (action == 1) {
            print_file_dialog(parent, filename);
        } else if (action == 2) {
            clear_staff_headquarters(staff_headquarters);
            Staff new_staff = create_staff_dialog(parent);
            add_staff(staff_headquarters, new_staff);
            for (int i = 0; i < staff_headquarters->quantity; i++) {
                char *string = report_staff_headquarters(staff_headquarters, i);
                add_to_file(filename,string);
            }
            // update_staff_list_view(list_view, store, staff_headquarters);
        } else {
            print_error_dialog(parent, "Error");
        }
    }
    exit(EXIT_SUCCESS);
}


void print_file_dialog(GtkWidget *parent, const gchar *filename) {
    // Создаем новое окно
    GtkWidget *dialog = gtk_dialog_new_with_buttons("File Content", GTK_WINDOW(parent), GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Создаем область с прокруткой, чтобы текст мог прокручиваться, если его много
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    // Создаем текстовое поле для отображения содержимого файла
    GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
    GtkWidget *text_view = gtk_text_view_new_with_buffer(buffer);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);

    // Добавляем текстовое поле в область с прокруткой
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_container_add(GTK_CONTAINER(content_area), scrolled_window);

    // Читаем содержимое файла и отображаем в текстовом поле
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char buffer[1000];
        while (fgets(buffer, 1000, file) != NULL) {
            GtkTextIter iter;
            gtk_text_buffer_get_end_iter(buffer, &iter);
            gtk_text_buffer_insert(buffer, &iter, buffer, -1);
        }
        fclose(file);
    } else {
        // Если не удалось открыть файл, отобразим сообщение об ошибке
        gtk_text_buffer_set_text(buffer, "Ошибка открытия файла", -1);
    }

    // Отображаем диалоговое окно
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void clear_staff_headquarters(StaffHeadquarters* staff_headquarters) {
    // Освобождаем память для каждого сотрудника и обнуляем количество сотрудников
    for (int i = 0; i < staff_headquarters->quantity; i++) {
        free(staff_headquarters->staff[i]);
    }
    staff_headquarters->quantity = 0;
}

char* report_staff_headquarters(StaffHeadquarters* staff_headquarters, int i) {
    char* report = (char*)malloc(1000 * sizeof(char));

    if (staff_headquarters->staff[i] != NULL) {
        strcpy(report, "");
        char temp[1000];
        sprintf(temp, "id: %d; Имя: %s; Статус: %s; Паспорт: %s; Должность: %s; Оклад: %d; Дата оформления: %s; Номер телефона: %s; ИНН: %s\n", 
                staff_headquarters->staff[i]->id, staff_headquarters->staff[i]->name, staff_headquarters->staff[i]->status, 
                staff_headquarters->staff[i]->passport, staff_headquarters->staff[i]->position, 
                staff_headquarters->staff[i]->salary, staff_headquarters->staff[i]->emp_date, 
                staff_headquarters->staff[i]->phone_num, staff_headquarters->staff[i]->inn);
        strncat(report, temp, 1000 - strlen(report) - 1);
    } else {
        strcpy(report, "Invalid staff\n");
    }

    return report;
}

void print_error_dialog(GtkWidget *parent, const gchar *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


int switch_system_admin_dialog(GtkApplication *app) {
    GtkWidget *dialog;
    
    // Создаем новое диалоговое окно для выбора подсистемы
    dialog = gtk_dialog_new_with_buttons("Выбор подсистемы", GTK_WINDOW(gtk_application_get_active_window(app)), GTK_DIALOG_MODAL, "Персонал", 1, "Заказы", 2, "Склад", 3, "Отмена", 0, NULL);

    // Отображаем сообщение пользователю
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    int chosen_system = 0; // По умолчанию, если пользователь не сделал выбор

    switch (result) {
        case 1:
            chosen_system = 1; // Персонал
            break;
        case 2:
            chosen_system = 2; // Заказы
            break;
        case 3:
            chosen_system = 3; // Склад
            break;
        default:
            chosen_system = 0; // Отмена или другие действия
            break;
    }

    // Уничтожаем созданное диалоговое окно
    gtk_widget_destroy(dialog);

    return chosen_system;
}


int calculate_salary(char* position) {
    int salary = 0;
    if (strcmp(position, "Менеджер") == 0) {
        salary = 60000;
    } else if (strcmp(position, "Инженер") == 0) {
        salary = 80000;
    } else if (strcmp(position, "Аналитик") == 0) {
        salary = 70000;
    } else if (strcmp(position, "Бухгалтер") == 0) {
        salary = 65000;
    }
    return salary;
}

void print_available_system_dialog(GtkApplication *app, const gchar *system_name) {
    // Создаем новое диалоговое окно для отображения информации о доступной системе
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_application_get_active_window(app)),
                                              GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_INFO,
                                              GTK_BUTTONS_OK,
                                              "Доступна система: %s", system_name);

    // Отображаем созданное диалоговое окно
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
