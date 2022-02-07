#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

bool shared_val = false;   // 共有変数

bool test_and_set(volatile bool* p) {
    return __sync_lock_test_and_set(p, 1);
}

void tas_release(volatile bool* p) {
    return __sync_lock_release(p);
}

void* worker(void *arg) {   // スレッド用関数
    volatile bool val = false;
    printf("worker: start.\n");

    while (val == false)
    {
        val = shared_val;
        sleep(1);   // このsleepがないとスレッドが終了しない
    }    

    printf("worker: exit.\n");

    return NULL;
}

int main(int argc, char* argv[]) {
    // スレッド生成
    pthread_t th;
    bool ret;

    if (pthread_create(&th, NULL, worker, NULL) != 0){
        perror("pthread_create");
        return -1;
    }

    sleep(3);

    printf("main: shared_val = %d\n", shared_val);
    printf("main: test_and_set exec.\n");

    ret = test_and_set(&shared_val);

    printf("main: test_and_set ret = %d/shared_val = %d\n", ret, shared_val);

    // スレッドの終了を待機
    if (pthread_join(th, NULL) != 0) {
        perror("pthread_join");
        return -1;
    }

    printf("pthread_join exit shared_val = %d\n", shared_val);

    printf("main: test_and_set 2 exec.\n");

    ret = test_and_set(&shared_val);

    printf("main: test_and_set 2 ret = %d/shared_val = %d\n", ret, shared_val);

    tas_release(&shared_val);

    printf("main: shared_val(after tas_release) = %d\n", shared_val);

}
