#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* print_message(void* arg) {
    printf("Hello from thread! Argument: %s\n", (char*)arg);
    return NULL;
}

int main() {
    pthread_t thread; // 쓰레드 식별자
    const char* message = "Thread argument"; // 전달할 인자

    // 쓰레드 생성
    if (pthread_create(&thread, NULL, print_message, (void*)message) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    pthread_join(thread, NULL);
    printf("Thread finished execution\n");
    return 0;
}
