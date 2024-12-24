#include <gtk/gtk.h>
#include <stdlib.h>

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    GtkEntry *entry = GTK_ENTRY(data);
    const gchar *current_text = gtk_entry_get_text(entry);
    gchar *new_text = g_strconcat(current_text, button_label, NULL);
    gtk_entry_set_text(entry, new_text);
    g_free(new_text);
}

static void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(data), "");
}

static void on_equal_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *expression = gtk_entry_get_text(GTK_ENTRY(data));
    gchar *result = g_strdup(expression);
    
    // 수식 계산 처리
    // 간단한 계산을 위해 `strtod`와 `sscanf`를 사용하여 계산
    double operand1 = 0, operand2 = 0;
    char operator = 0;
    
    // 간단한 사칙 연산만 처리 (두 수와 연산자만 처리)
    if (sscanf(expression, "%lf %c %lf", &operand1, &operator, &operand2) == 3) {
        double calc_result = 0;
        switch (operator) {
            case '+':
                calc_result = operand1 + operand2;
                break;
            case '-':
                calc_result = operand1 - operand2;
                break;
            case '*':
                calc_result = operand1 * operand2;
                break;
            case '/':
                if (operand2 != 0) {
                    calc_result = operand1 / operand2;
                } else {
                    gtk_entry_set_text(GTK_ENTRY(data), "에러: 0으로 나눔");
                    return;
                }
                break;
            default:
                gtk_entry_set_text(GTK_ENTRY(data), "잘못된 연산자");
                return;
        }
        gchar result_text[64];
        g_snprintf(result_text, sizeof(result_text), "%.2f", calc_result);
        gtk_entry_set_text(GTK_ENTRY(data), result_text);
    } else {
        gtk_entry_set_text(GTK_ENTRY(data), "잘못된 입력");
    }

    g_free(result);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *entry;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    // 윈도우 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "계산기");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 그리드 레이아웃 생성
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 텍스트 입력 필드 생성
    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // 숫자 버튼들 생성
    const char *buttons[16] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "C", "0", "=", "+"
    };

    for (int i = 0; i < 16; i++) {
        button = gtk_button_new_with_label(buttons[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);
        int row = i / 4 + 1;
        int col = i % 4;
        gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
    }

    // = 버튼 연결
    GtkWidget *equal_button = gtk_button_new_with_label("=");
    g_signal_connect(equal_button, "clicked", G_CALLBACK(on_equal_button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), equal_button, 3, 5, 1, 1);

    // 지우기 버튼 연결
    GtkWidget *clear_button = gtk_button_new_with_label("C");
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), clear_button, 0, 5, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
