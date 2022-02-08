#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10  // スレッド数
#define NUM_LOOP    10  // スレッド内のループ数

int count = 0;  // セマフォのカウント数

void* th(void* arg) {
    int n = (int)arg;

    // 名前付きセマフォを開く
    sem_t* s = sem_open("/mysemaphore", 0);
    if (s == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    for (int i = 0; i < NUM_LOOP; i++) {
        // 待機
        if (sem_wait(s) == -1) {
            perror("sem_wait");
            exit(1);
        }

        // カウンタをアトミックにインクリメント
        __sync_fetch_and_add(&count, 1);
//        printf("count = %d\n", count);
        printf("thread[%d]: count = %d\n", n, count);

        // 10msスリープ
        usleep(10000);

        // カウンタをアトミックにデクリメント
        __sync_fetch_and_sub(&count, 1);

        // セマフォの数を増やし
        // クリティカルセクションを抜ける
        if (sem_post(s) == -1) {
            perror("sem_post");
            exit(1);
        }
    }

    // セマフォを閉じる
    if (sem_close(s) == 1) {
        perror("sem_close");
    }

    return NULL;
}


int main(int argc, char* argv[]) {
    // 名前付きセマフォを開く。ない場合は生成
    // 自分とグループが利用可能なセマフォで、
    // クリティカルセクションに入れるプロセスの上限は3
    sem_t *s = sem_open("/mysemaphore", O_CREAT, 0660, 3);
    if (s == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // スレッド生成
    pthread_t v[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
//        pthread_create(&v[i], NULL, th, NULL);
        pthread_create(&v[i], NULL, th, (void*)i);
    }

    // join
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(v[i], NULL);
    }

    // セマフォを閉じる
    if (sem_close(s) == -1)
        perror("sem_close");

    // セマフォを破棄
    if (sem_unlink("/mysemaphore") == -1)
        perror("sem_unlink");

    printf("Bye.\n");

    return 0;
}

