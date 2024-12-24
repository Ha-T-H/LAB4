// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define BUF_SIZE 1024
#define MAX_CLIENTS 10

int main() {
    int server_socket, client_socket, max_sd, activity, new_socket, valread;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    int client_sockets[MAX_CLIENTS]; // 클라이언트 소켓 배열
    fd_set readfds;

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

    // 클라이언트 소켓 초기화
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        FD_ZERO(&readfds); // 읽기 가능한 소켓을 추적하기 위한 셋 초기화
        FD_SET(server_socket, &readfds); // 서버 소켓을 추가

        max_sd = server_socket; // 최대 소켓 디스크립터를 찾기 위한 변수

        // 클라이언트 소켓을 셋에 추가
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];

            // 유효한 클라이언트 소켓만 추가
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // select() 호출하여 소켓들에 대한 활동을 대기
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select() 실패");
            return 1;
        }

        // 새로운 클라이언트 연결
        if (FD_ISSET(server_socket, &readfds)) {
            new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
            if (new_socket < 0) {
                perror("새로운 클라이언트 연결 실패");
                return 1;
            }

            printf("새로운 클라이언트 연결: %d\n", new_socket);

            // 클라이언트 소켓 배열에 새 소켓 추가
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // 클라이언트로부터 메시지를 받음
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, BUF_SIZE);
                if (valread == 0) {
                    // 클라이언트가 연결을 끊으면 소켓 닫기
                    getpeername(sd, (struct sockaddr*)&client_addr, &client_addr_len);
                    printf("클라이언트 %d 연결 종료\n", sd);
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    printf("클라이언트 %d 메시지: %s\n", sd, buffer);

                    // 받은 메시지를 다른 모든 클라이언트에게 전송
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        int other_sd = client_sockets[j];
                        if (other_sd != 0 && other_sd != sd) {
                            send(other_sd, buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
