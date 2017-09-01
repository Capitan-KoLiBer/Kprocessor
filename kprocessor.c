#include "kprocessor.h"

typedef struct Message{
    void (*function)(void *);
    void *arg;
} Message;

int equals(void *a , void *b){
    return a == b;
}

void looper_start(Kprocessor *kprocessor){
    while(1){
        Message *message = kmsg_recv(kprocessor->pool_queue,KMSG_MODE_WAIT);
        message->function(message->arg);
    }
}

Kprocessor *kprocessor_new(KprocessorPoolType pool_type , int pool_size){
    Kprocessor *kprocessor = malloc(sizeof(Kprocessor));
    kprocessor->pool_type = pool_type;
    kprocessor->pool_size = pool_size;
    kprocessor->pool_queue = kmsg_new(KMSG_KEY_RANDOM , NULL);
    kprocessor->pool_queue = kmsg_create(kprocessor->pool_queue , sizeof(Message));
    kprocessor->pool_list = klist_new(sizeof(int) , equals);
    return kprocessor;
}

void kprocessor_start(Kprocessor *kprocessor){
    if(kprocessor->pool_type == KPROCESSOR_POOL_TYPE_THREAD){
        for(int cursor = 0 ; cursor < kprocessor->pool_size ; cursor++){
            pthread_t tid;
            pthread_create(&tid , NULL , (void *(*)(void *)) looper_start, kprocessor);
            kprocessor->pool_list = klist_add(kprocessor->pool_list , (void *) tid);
        }
    }else{
        for(int cursor = 0 ; cursor < kprocessor->pool_size ; cursor++){
            int pid = fork();
            if(pid == 0){
                looper_start(kprocessor);
                break;
            }else if(pid > 0){
                kprocessor->pool_list = klist_add(kprocessor->pool_list , (void *) pid);
            }else{
                exit(0);
            }
        }
    }
}

void kprocessor_post(Kprocessor *kprocessor , void (*function)(void*) , void *arg){
    Message message;
    message.function = function;
    message.arg = arg;
    kmsg_send(kprocessor->pool_queue , &message);
}

void kprocessor_stop(Kprocessor *kprocessor){
    if(kprocessor->pool_type == KPROCESSOR_POOL_TYPE_THREAD){
        int length = klist_length(kprocessor->pool_list) - 1;
        while(length >= 0){
            pthread_cancel((pthread_t) klist_get(kprocessor->pool_list , length));
            length--;
        }
    }else{
        int length = klist_length(kprocessor->pool_list) - 1;
        while(length >= 0){
            kill((__pid_t) klist_get(kprocessor->pool_list , length), SIGKILL);
            length--;
        }
    }
}

void kprocessor_free(Kprocessor *kprocessor){
    if(kprocessor != NULL){
        kmsg_destroy(kprocessor->pool_queue);
        kmsg_free(kprocessor->pool_queue);
        klist_free(kprocessor->pool_list);
        free(kprocessor);
    }
}