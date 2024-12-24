#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 5

int buf[BUF_SIZE];  // 버퍼
int count = 0;      // 버퍼 항목 개수
int in = 0;         // 삽입 위치
int out = 0;        // 제거 위치

pthread_mutex_t mtx;       // 뮤텍스
pthread_cond_t not_empty;  // 버퍼 비어있지 않음
pthread_cond_t not_full;   // 버퍼 가득 찬 것 아님

// 생산자 함수
void* producer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        int item = rand() % 100;  // 랜덤 항목
        pthread_mutex_lock(&mtx);

        // 버퍼가 가득 차면 대기
        while (count == BUF_SIZE)
            pthread_cond_wait(&not_full, &mtx);

        // 항목 삽입
        buf[in] = item;
        printf("P%d: produced %d\n", id, item);
        in = (in + 1) % BUF_SIZE;
        count++;

        // 소비자에게 알림
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mtx);

        sleep(1);  // 생산 속도
    }

    return NULL;
}

// 소비자 함수
void* consumer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mtx);

        // 버퍼가 비어 있으면 대기
        while (count == 0)
            pthread_cond_wait(&not_empty, &mtx);

        // 항목 소비
        int item = buf[out];
        printf("C%d: consumed %d\n", id, item);
        out = (out + 1) % BUF_SIZE;
        count--;

        // 생산자에게 알림
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mtx);

        sleep(2);  // 소비 속도
    }

    return NULL;
}

int main() {
    pthread_t prod[2], cons[2];
    int ids[2] = {1, 2};

    // 뮤텍스 및 조건 변수 초기화
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // 생산자 및 소비자 쓰레드 생성
    for (int i = 0; i < 2; i++) {
        pthread_create(&prod[i], NULL, producer, &ids[i]);
        pthread_create(&cons[i], NULL, consumer, &ids[i]);
    }

    // 쓰레드 종료 대기
    for (int i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }

    // 뮤텍스 및 조건 변수 제거
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
