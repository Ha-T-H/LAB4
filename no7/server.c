// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void handle_get(int client_socket);
void handle_post(int client_socket, char *data);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("소켓 생성 실패");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("바인딩 실패");
        return 1;
    }

    // 연결 대기
    if (listen(server_socket, 3) < 0) {
        perror("대기 실패");
        return 1;
    }

    printf("서버가 %d 포트에서 대기 중...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("연결 수락 실패");
            continue;
        }

        // 클라이언트 요청 받기
        int read_size = recv(client_socket, buffer, BUF_SIZE - 1, 0);
        if (read_size < 0) {
            perror("요청 받기 실패");
            close(client_socket);
            continue;
        }
        buffer[read_size] = '\0';

        // 요청 파싱
        if (strncmp(buffer, "GET", 3) == 0) {
            handle_get(client_socket);
        } else if (strncmp(buffer, "POST", 4) == 0) {
            char *data = strstr(buffer, "\r\n\r\n") + 4; // POST 데이터 부분 추출
            handle_post(client_socket, data);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}

void handle_get(int client_socket) {
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                      "<html><body><h1>GET 요청 처리</h1><p>GET 요청이 성공적으로 처리되었습니다.</p></body></html>";

    send(client_socket, response, sizeof(response) - 1, 0);
}

void handle_post(int client_socket, char *data) {
    char response[BUF_SIZE];

    // POST 요청에서 받은 데이터를 출력
    snprintf(response, sizeof(response), 
             "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
             "<html><body><h1>POST 요청 처리</h1><p>받은 데이터: %s</p></body></html>", data);

    send(client_socket, response, strlen(response), 0);
}
