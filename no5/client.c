// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[BUF_SIZE];
    char response[BUF_SIZE];

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

    while (1) {
        printf("메시지를 입력하세요: ");
        fgets(message, BUF_SIZE, stdin);

        // "exit" 입력 시 종료
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }

        // 서버로 메시지 전송
        send(sock, message, strlen(message), 0);

        // 서버로부터 응답 받기
        int read_size = recv(sock, response, BUF_SIZE, 0);
        if (read_size > 0) {
            response[read_size] = '\0';
            printf("서버 응답: %s\n", response);
        }
    }

    close(sock);
    return 0;
}
