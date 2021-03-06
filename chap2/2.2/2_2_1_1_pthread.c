#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10   // 生成するスレッドの数


// スレッド用関数
void *thread_func(void* arg) {
    int id = (int)arg;

    for (int i = 0; i < 5; i++){
        printf("id = %d, i = %d\n", id, i);
        sleep(1);
    }

    return "finished!";
}


int main(int argc, char* argv[]) {
    pthread_t v[NUM_THREADS];

    // スレッド生成
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&v[i], NULL, thread_func, (void*)i) != 0){
            perror("pthread_create");
            return -1;
        }
    }

    // スレッドの終了を待機
    for (int i = 0; i < NUM_THREADS; i++){
        char* ptr;

        if (pthread_join(v[i], (void**)&ptr) == 0) {
            printf("msg[%d] = %s\n", i, ptr);
        } else {
            perror("pthread_join");
            return -1;
        }
    }

    return 0;
}

