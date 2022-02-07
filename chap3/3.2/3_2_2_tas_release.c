#include <stdint.h>
#include <stdbool.h>

void tas_release(volatile bool* p) {
    return __sync_lock_release(p);
}