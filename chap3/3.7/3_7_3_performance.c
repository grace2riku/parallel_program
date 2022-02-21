#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// do_lock関数の中身を切り替え
#if defined(RWLOCK)
#include "rwlock.c"
#elif defined(RWLOCK_WR)
#include "rwlock_wr.c"
#elif defined(MUTEX)
#include "mutex.c"
#elif defined(EMPTY)
#include "empty.c"
#else
#error define(do_lock)
#endif

#include "barrier.c"

volatile int flag = 0;  // このフラグが0の間ループ

// バリア同期用変数
volatile int waiting_1 = 0;
volatile int waiting_2 = 0;

// NUM_THREADはワーカスレッドとタイマスレッドの合計
// よってワーカスレッドの数はNUM_THREAD - 1となる
uint64_t count[NUM_THREAD - 1];

void* worker(void* arg) {   // ワーカースレッド用関数
    uint64_t id = (uint64_t)arg;
    barrier(&waiting_1, NUM_THREAD);    // バリア同期

    uint64_t n = 0;
    while (flag == 0) {
        do_lock();  // 必要ならロックを獲得して待機
        n++;
    }
    count[id] = n;  // 何回ループしたか記録

    barrier(&waiting_2, NUM_THREAD);    // バリア同期

    return NULL;
}

void* timer(void* arg) {    // タイマスレッド用関数
    barrier(&waiting_1, NUM_THREAD);    // バリア動機

    sleep(180);
    flag = 1;

    barrier(&waiting_2, NUM_THREAD);    // バリア同期
    for (int i = 0; i < NUM_THREAD - 1; i++) {
        printf("count[%d] = %llu\n", i, count[i]);
    }

    return NULL;
}

int main(){
    printf("NUM_THREAD=%d, HOLDTIME=%d\n", NUM_THREAD, HOLDTIME);

    // ワーカスレッド起動
    for (uint64_t i = 0; i < NUM_THREAD - 1; i++) {
        pthread_t th;
        pthread_create(&th, NULL, worker, (void*)i);
        pthread_detach(th);        
    }

    // タイマスレッド起動
    pthread_t th;
    pthread_create(&th, NULL, timer, NULL);
    pthread_join(th, NULL);

    return 0;
}
