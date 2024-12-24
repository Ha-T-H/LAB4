#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("버튼이 클릭되었습니다!\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    // GTK+ 초기화
    gtk_init(&argc, &argv);

    // 윈도우 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "no8 GUI GTK 프로그램");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 버튼 생성
    button = gtk_button_new_with_label("클릭하세요");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // 버튼을 윈도우에 추가
    gtk_container_add(GTK_CONTAINER(window), button);

    // 윈도우 보이기
    gtk_widget_show_all(window);

    // GTK+ 메인 루프 시작
    gtk_main();

    return 0;
}
