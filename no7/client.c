// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void send_get_request(int sock);
void send_post_request(int sock, char *data);

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("소켓 생성 실패");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 서버에 연결
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("서버 연결 실패");
        return 1;
    }

    printf("서버에 연결됨\n");

    // GET 요청 보내기
    send_get_request(sock);
    recv(sock, buffer, BUF_SIZE, 0);
    printf("서버 응답 (GET):\n%s\n", buffer);

    // POST 요청 보내기
    char post_data[] = "username=guest&password=1234";
    send_post_request(sock, post_data);
    recv(sock, buffer, BUF_SIZE, 0);
    printf("서버 응답 (POST):\n%s\n", buffer);

    close(sock);
    return 0;
}

void send_get_request(int sock) {
    char request[] = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";
    send(sock, request, sizeof(request) - 1, 0);
}

void send_post_request(int sock, char *data) {
    char request[BUF_SIZE];
    snprintf(request, sizeof(request),
             "POST / HTTP/1.1\r\nHost: 127.0.0.1\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %lu\r\nConnection: close\r\n\r\n%s",
             strlen(data), data);
    send(sock, request, strlen(request), 0);
}
