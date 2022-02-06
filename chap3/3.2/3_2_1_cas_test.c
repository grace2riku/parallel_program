#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

uint64_t shared_val = 0;   // 共有変数

bool compare_and_swap(uint64_t* p, uint64_t val, uint64_t newval) {
    if (*p != val) {
        return false;
    }
    *p = newval;
    return true;
}

void* worker(void *arg) {   // スレッド用関数
    bool ret = false;

    printf("worker: start.\n");

    while (ret != true)
    {
        ret = compare_and_swap(&shared_val, 1, 2);
    }    

    printf("worker: exit.\n");

    return NULL;
}

int main(int argc, char* argv[]) {
    // スレッド生成
    pthread_t th;

    if (pthread_create(&th, NULL, worker, NULL) != 0){
        perror("pthread_create");
        return -1;
    }

    sleep(5);
    shared_val = 1;
    printf("main: shared_val 0to1.shared_val = %llu\n", shared_val);

    // スレッドの終了を待機
    if (pthread_join(th, NULL) != 0) {
        perror("pthread_join");
        return -1;
    }

    printf("pthread_join exit shared_val = %llu\n", shared_val);

}
