#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void* reader(void* arg) {   // Reader用関数
    if (pthread_rwlock_rdlock(&rwlock) != 0) {
        perror("pthread_rwlock_rdlock"); exit(-1);
    }

    // クリティカルセクション（読み込みのみ）
    printf("reader critical section...\n");
    sleep(3);

    if (pthread_rwlock_unlock(&rwlock) != 0) {
        perror("pthread_rwlock_unlock"); exit(-1);
    }

    return NULL;
}

void* writer(void* arg) {   // Writer用関数
    if (pthread_rwlock_wrlock(&rwlock) != 0) {
        perror("pthread_rwlock_wrlock"); exit(-1);
    }

    // クリティカルセクション（読み書き）
    printf("writer critical section...\n");
    sleep(5);

    if (pthread_rwlock_unlock(&rwlock) != 0) {
        perror("pthread_rwlock_unlock"); exit(-1);
    }

    return NULL;   
}


int main(int argc, char* argv[]) {
    // スレッド生成
    pthread_t rd, wr;
    pthread_create(&rd, NULL, reader, NULL);
    pthread_create(&wr, NULL, writer, NULL);

    // スレッドの終了を待機
    pthread_join(rd, NULL);
    printf("reader join exit.\n");
    pthread_join(wr, NULL);
    printf("writer join exit.\n");

    // RWロックオブジェクトを解放
    if (pthread_rwlock_destroy(&rwlock) != 0) {
        perror("pthread_rwlock_destroy"); return -1;
    }

    printf("Bye\n");

    return 0;
}
