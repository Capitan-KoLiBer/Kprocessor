# Kprocessor
Very powerful Thread pool and Process pool library in C based on Unix processes and POSIX threads

## headers

1. kprocessor.h   

## What is Kprocessor
Kprocessor is a library for creating and managing Thread pools and Process pools in C language .

Kprocessor uses Unix processes for creating Process pools and POSIX threads for Thread pools

By posting a function address and an argument you can add task to queue for processing 

It's very simple :) 

test it


# Functions
```
1. Kprocessor *kprocessor_new(KprocessorPoolType pool_type , int pool_size);
    creating new pool 
    
2. void kprocessor_start(Kprocessor *kprocessor);
    starting the pool
    
3. void kprocessor_post(Kprocessor *kprocessor , void (*function)(void*) , void *arg);
    posting task to the pool
    
4. void kprocessor_stop(Kprocessor *kprocessor);
    stopping the pool
    
5. void kprocessor_free(Kprocessor *kprocessor);
    freeing the pool memory
```

# examples 
```
check examples.c source file
```


# Min deps
```
1. CMake >= 2.8
2. Unix base OS
3. libkstd.so
4. libkipc.so
```

# Usage
```
git clone https://github.com/Capitan-KoLiBer/Kprocessor.git
mkdir build
cd build
cmake ..
make 
```
