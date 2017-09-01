#ifndef KSTD_KPROCESSOR_H
#define KSTD_KPROCESSOR_H

#include <signal.h>
#include "klist.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "kmsg.h"

typedef enum KprocessorPoolType{
    KPROCESSOR_POOL_TYPE_THREAD,
    KPROCESSOR_POOL_TYPE_PROCESS
} KprocessorPoolType;

typedef struct Kprocessor {
    KprocessorPoolType pool_type;
    int pool_size;
    Kmsg *pool_queue;
    Klist *pool_list;
} Kprocessor;

Kprocessor *kprocessor_new(KprocessorPoolType pool_type , int pool_size);

void kprocessor_start(Kprocessor *kprocessor);

void kprocessor_post(Kprocessor *kprocessor , void (*function)(void*) , void *arg);

void kprocessor_stop(Kprocessor *kprocessor);

void kprocessor_free(Kprocessor *kprocessor);

#endif //KSTD_KPROCESSOR_H
