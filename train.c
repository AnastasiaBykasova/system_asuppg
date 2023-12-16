#include <gtk/gtk.h> /* подключаем GTK+ */

void welcome (GtkButton *button, gpointer data);

/* эта функция будет выполнена первой */
int main( int argc, char *argv[])
{
        /* тут мы объявим переменные */
        GtkWidget *window;
        // GtkWidget *button;


        /* запускаем GTK+ */
        gtk_init(&argc, &argv);

        /* тут будет код нашего приложения */
        /* создать новый виджет - окно */
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        /* дать окну заголовок */
        gtk_window_set_title(GTK_WINDOW(window), "Введение в GTK");
        /* установить внутренние границы (читайте ниже) */
        gtk_container_set_border_width (GTK_CONTAINER(window), 50);

        /* создать кнопку с меткой */

        button = gtk_button_new_with_label("Привет, ХабраХабр!");
        /* упаковать нашу кнопку в окно */
        gtk_container_add(GTK_CONTAINER(window), button);

        /* когда пользователь кликнет по ней, то вызвать ф-цию welcome */
        g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(welcome), NULL);

        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

        /* передаём управление GTK+ */
        gtk_main();

        return 0;
}



/* выводит приветствие */
void welcome (GtkButton *button, gpointer data)
{
        /* виджеты */
        GtkWidget *dialog;
        GtkWidget *label;
        GtkWidget *content_area;

        /* создать диалог */
        dialog = gtk_dialog_new_with_buttons("Ошибка LOL!!!111",
                                             NULL,
                                             GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_STOCK_OK,
                                             GTK_RESPONSE_ACCEPT,
                                             NULL);

        /* получить контейнер, в который будем пихать метку */
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        /* сама метка */
        label = gtk_label_new("\n\nИ тебе привет, %username!%");
        gtk_container_add(GTK_CONTAINER(content_area), label);
        gtk_widget_show(label);

        /* запускаем диалог */
        gtk_dialog_run(GTK_DIALOG(dialog));
        /* а потом прячем */
        gtk_widget_destroy(dialog);
}