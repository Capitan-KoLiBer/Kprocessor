#include <stdio.h>
#include "kprocessor.h"

void test(void *data){
    printf("RESULT = %d\n", (int) data);fflush(stdout);
}

int main() {
    Kprocessor *pool = kprocessor_new(KPROCESSOR_POOL_TYPE_PROCESS, 1);
    kprocessor_start(pool);
    sleep(1);
    kprocessor_post(pool,test,10);
    sleep(1);
    kprocessor_stop(pool);
    kprocessor_free(pool);
    return 0;
}