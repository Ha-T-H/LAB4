// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

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

    // 클라이언트 연결 대기
    if (listen(server_socket, 3) < 0) {
        perror("대기 실패");
        return 1;
    }

    printf("서버가 %d 포트에서 대기 중...\n", PORT);

    // 클라이언트 연결 수락
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        perror("연결 수락 실패");
        return 1;
    }

    printf("클라이언트가 연결됨\n");

    // 클라이언트로부터 메시지를 받음
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int read_size = recv(client_socket, buffer, BUF_SIZE, 0);
        if (read_size <= 0) {
            break;
        }

        printf("받은 메시지: %s\n", buffer);

        // 클라이언트에게 메시지 전송
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
