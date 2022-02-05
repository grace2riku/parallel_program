#include <pthread.h>
#include <stdio.h>

//#define NUM_THREADS 10   // 生成するスレッドの数
#define NUM_THREADS 8   // 生成するスレッドの数

void barrier(volatile int* cnt, int max) {
    __sync_fetch_and_add(cnt, 1);

    printf("before barrier: thread_id = %d/*cnt = %d\n", pthread_self(), *cnt);

    while (*cnt < max);
}

volatile int num = 0;   // 共有変数


void* worker(void *arg) {   // スレッド用関数
    barrier(&num, NUM_THREADS);  // 全スレッドがここまで到達するまで待つ

    // 何らかの処理
    printf("after barrier: thread_id = %d/num = %d\n", pthread_self(),num);

    return NULL;
}


int main(int argc, char* argv[]) {
    // スレッド生成
    pthread_t th[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++){
        if (pthread_create(&th[i], NULL, worker, NULL) != 0){
            perror("pthread_create");
        }
    }

    // スレッドの終了を待機
    for (int i = 0; i < NUM_THREADS; i++){
        if (pthread_join(th[i], NULL) != 0) {
            perror("pthread_join");
            return -1;
        }
    }
    
}

