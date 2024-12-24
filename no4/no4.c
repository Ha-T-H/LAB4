#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CLIENT_COUNT 3
#define BUF_SIZE 256

char msg[BUF_SIZE];
pthread_mutex_t mtx;
pthread_cond_t cond;
int ready = 0;

void* client(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mtx);
        while (!ready)
            pthread_cond_wait(&cond, &mtx);

        printf("클라이언트 %d: 받은 메시지: %s\n", id, msg);
        pthread_mutex_unlock(&mtx);

        sleep(1);
    }

    return NULL;
}

void* server(void* arg) {
    while (1) {
        char input[BUF_SIZE];

        printf("클라이언트들로 보낼 메시지를 입력하세요: ");
        fgets(input, BUF_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        pthread_mutex_lock(&mtx);
        strcpy(msg, input);
        ready = 1;

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mtx);

        sleep(2);
    }

    return NULL;
}

int main() {
    pthread_t srv;
    pthread_t cli[CLIENT_COUNT];
    int ids[CLIENT_COUNT] = {1, 2, 3};

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&srv, NULL, server, NULL);

    for (int i = 0; i < CLIENT_COUNT; i++) {
        pthread_create(&cli[i], NULL, client, &ids[i]);
    }

    pthread_join(srv, NULL);
    for (int i = 0; i < CLIENT_COUNT; i++) {
        pthread_join(cli[i], NULL);
    }

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

    return 0;
}
