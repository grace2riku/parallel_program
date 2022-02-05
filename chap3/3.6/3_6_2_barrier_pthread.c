#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t barrier_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;

//#define NUM_THREADS 10   // 生成するスレッドの数
#define NUM_THREADS 8   // 生成するスレッドの数

volatile int num = 0;   // 共有変数

void barrier(volatile int* cnt, int max) {
    if (pthread_mutex_lock(&barrier_mut) != 0) {
        perror("pthread_mutex_lock"); exit(-1);
    }

    (*cnt)++;

    if (*cnt == max) {
        // 全プロセスが揃ったので通知
        if (pthread_cond_broadcast(&barrier_cond) != 0) {
            perror("pthread_cond_broadcast"); exit(-1);
        }
    } else {
        do {    // 全スレッドが揃うまで待機
            if (pthread_cond_wait(&barrier_cond, &barrier_mut) != 0) {
                perror("pthread_cond_wait"); exit(-1);
            }
        } while (*cnt < max);   // 疑似覚醒のための条件
    }

    if (pthread_mutex_unlock(&barrier_mut) != 0) {
        perror("pthread_mutex_unlock"); exit(-1);
    }
}

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
