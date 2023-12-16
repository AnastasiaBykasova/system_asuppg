// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "my_lib.h"
// #include <gtk/gtk.h>
// void the_end(void);


// Staff create_staff();
// char* report_staff_headquarters(StaffHeadquarters* staff_headquarters, int i);
// void add_staff(StaffHeadquarters *staff_headquarters, Staff new_staff)
// // void add_staff(StaffHeadquarters* staff_headquarters, Staff staff);
// void print_file(char* filename);
// void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters);
// void add_staff_to_list_store(GtkListStore *store, const Staff *staff);
// void clear_staff_headquarters(StaffHeadquarters* staff_headquarters);
// int switch_action();
// void add_to_file(char* filename, char* report);

// void print_error();
// int calculate_salary(char* position);

// void staff_proccess(char* filename, int action, StaffHeadquarters* staff_headquarters) {
//     int for_exit = 0;
//     for_exit = atexit(the_end);
//     while((action = switch_action()) != 3 && !for_exit) {
//         if (action == 1) {
//             print_file(filename);
//         } else if (action == 2) {
//             clear_staff_headquarters(staff_headquarters);
//             Staff new_staff = create_staff();
//             add_staff(staff_headquarters, new_staff);
//             for (int i = 0; i < staff_headquarters->quantity; i++) {
//                 char* string = report_staff_headquarters(staff_headquarters, i);
//                 add_to_file(filename, string);
//             }
//         } else {
//             print_error();
//         }
//     }
//     exit(EXIT_SUCCESS);
// }

// Staff create_staff() {
//     Staff new_staff;

//     printf("Введите имя сотрудника: ");
//     fflush(stdout);
//     scanf("%s", new_staff.name);

//     printf("Введите статус сотрудника: ");
//     fflush(stdout);
//     scanf("%s", new_staff.status);

//     printf("Введите дату оформления (mm/dd/yyyy): ");
//     fflush(stdout);
//     scanf("%s", new_staff.emp_date);

//     printf("Введите паспорт сотрудника: ");
//     fflush(stdout);
//     scanf("%s", new_staff.passport);

//     printf("Введите ИНН сотрудника: ");
//     fflush(stdout);
//     scanf("%s", new_staff.inn);
    
//     printf("Введите номер телефона сотрудника: ");
//     fflush(stdout);
//     scanf("%s", new_staff.phone_num);

//     printf("Введите должность сотрудника: ");
//     fflush(stdout);
//     scanf("%s", new_staff.position);

//     new_staff.salary = calculate_salary(new_staff.position);

//     return new_staff;
// }

// // Функция создания нового сотрудника с использованием диалоговых окон GTK+
// Staff create_staff_dialog(GtkWidget *parent) {
//     Staff new_staff;

//     // Создаем диалоговые окна для ввода информации о сотруднике
//     GtkWidget *dialog = gtk_dialog_new_with_buttons("Create Staff", GTK_WINDOW(parent), GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
//     GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

//     // Добавляем поля для ввода информации
//     // Например, для имени:
//     GtkWidget *label = gtk_label_new("Имя сотрудника:");
//     gtk_container_add(GTK_CONTAINER(content_area), label);
//     GtkWidget *entry_name = gtk_entry_new();
//     gtk_container_add(GTK_CONTAINER(content_area), entry_name);

//     // Аналогично добавляем другие поля для ввода информации

//     // Отображаем диалоговое окно и ждем ввода от пользователя
//     gtk_widget_show_all(dialog);
//     gint result = gtk_dialog_run(GTK_DIALOG(dialog));

//     if (result == GTK_RESPONSE_OK) {
//         // Получаем введенные значения
//         const gchar *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
//         // Аналогично для других полей

//         // Заполняем поля нового сотрудника
//         strcpy(new_staff.name, name);
//         // Аналогично для других полей
//         // Вычисляем оклад на основе должности
//         new_staff.salary = calculate_salary(new_staff.position);
//     }
//     gtk_widget_destroy(dialog); // Уничтожаем диалоговое окно

//     return new_staff;
// }

// void add_staff(StaffHeadquarters *staff_headquarters, Staff new_staff) {
//     if (staff_headquarters->quantity < MAX_AMOUNT_LENGTH) {
//         staff_headquarters->staff[staff_headquarters->quantity] = (Staff*)malloc(sizeof(Staff));
//         *(staff_headquarters->staff[staff_headquarters->quantity]) = new_staff;
//         staff_headquarters->quantity++;
//     } else {
//         printf("Достигнуто максимально возможное количество сотрудников\n");
//     }
// }

// void add_staff_to_list_store(GtkListStore *store, const Staff *staff) {
//     GtkTreeIter iter;
//     gtk_list_store_append(store, &iter);
//     gtk_list_store_set(store, &iter, COLUMN_ID, staff->id, COLUMN_NAME, staff->name, COLUMN_STATUS, staff->status, COLUMN_POSITION, staff->position, COLUMN_SALARY, staff->salary, -1);
// }

// char* report_staff_headquarters(StaffHeadquarters* staff_headquarters, int i) {
//     char* report = (char*)malloc(1000 * sizeof(char));

//     if (staff_headquarters->staff[i] != NULL) {
//         strcpy(report, "");
//         char temp[1000];
//         sprintf(temp, "id: %d; Имя: %s; Статус: %s; Паспорт: %s; Должность: %s; Оклад: %d; Дата оформления: %s; Номер телефона: %s; ИНН: %s\n", staff_headquarters->staff[i]->id, staff_headquarters->staff[i]->name, staff_headquarters->staff[i]->status, staff_headquarters->staff[i]->passport, staff_headquarters->staff[i]->position, staff_headquarters->staff[i]->salary, staff_headquarters->staff[i]->emp_date, staff_headquarters->staff[i]->phone_num, staff_headquarters->staff[i]->inn);
//         strncat(report, temp, 1000 - strlen(report) - 1);
//     } else {
//         strcpy(report, "Invalid staff\n");
//     }

//     return report;
// }

// int calculate_salary(char* position) {
//     int salary = 0;
//     if (strcmp(position, "Менеджер") == 0) {
//         salary = 60000;
//     } else if (strcmp(position, "Инженер") == 0) {
//         salary = 80000;
//     } else if (strcmp(position, "Аналитик") == 0) {
//         salary = 70000;
//     } else if (strcmp(position, "Бухгалтер") == 0) {
//         salary = 65000;
//     }
//     return salary;
// }

// void clear_staff_headquarters(StaffHeadquarters* staff_headquarters) {
//     for (int i = 0; i < staff_headquarters->quantity; i++) {
//         free(staff_headquarters->staff[i]);
//     }
//     staff_headquarters->quantity = 0;
// }


// // void the_end(void) {   
// //    puts("Работа завершена.");
// // }

// // Функция для обновления списка сотрудников в интерфейсе
// void update_staff_list_view(GtkWidget *list_view, GtkListStore *store, StaffHeadquarters *staff_headquarters) {
//     // Очищаем список
//     gtk_list_store_clear(store);

//     // Добавляем новых сотрудников в список
//     for (int i = 0; i < staff_headquarters->quantity; i++) {
//         add_staff_to_list_store(store, staff_headquarters->staff[i]);
//     }
// }


// // Функция для обработки события создания нового сотрудника
// static void create_staff_button_clicked(GtkWidget *widget, gpointer data) {
//     GtkWidget *parent = GTK_WIDGET(data);
//     Staff new_staff = create_staff_dialog(parent);
//     add_staff(staff_headquarters, new_staff);
//     update_staff_list_view(list_view, store, staff_headquarters);
// }

// // Обновленная функция staff_proccess, использование GTK+
// void staff_proccess_dialog(GtkWidget *parent, char *filename, int action, StaffHeadquarters *staff_headquarters) {
//     int for_exit = 0;
//     for_exit = atexit(the_end);

//     while ((action = switch_action()) != 3 && !for_exit) {
//         if (action == 1) {
//             print_file_dialog(parent, filename);
//         } else if (action == 2) {
//             clear_staff_headquarters(staff_headquarters);
//             Staff new_staff = create_staff_dialog(parent);
//             add_staff(staff_headquarters, new_staff);
//             for (int i = 0; i < staff_headquarters->quantity; i++) {
//                 char *string = report_staff_headquarters(staff_headquarters, i);
//                 add_to_file(filename,string);
//             }
//             update_staff_list_view(list_view, store, staff_headquarters);
//         } else {
//             print_error_dialog(parent, "Error");
//         }
//     }
//     exit(EXIT_SUCCESS);
// }