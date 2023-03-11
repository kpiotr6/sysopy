#pragma once
#include<stdlib.h>
#include <stdio.h>
#define BUFFER 255
typedef struct M{
    char** blocks;
    int max;
    int current;
}Memory;

#ifdef DYNAMIC
#include <stdlib.h>
#include <dlfcn.h>
Memory* (*init)(int);
void (*count)(Memory*,char*);
char* (*show)(Memory *m, int i);
void (*delete_all)(Memory *m,int j);
void (*destroy)(Memory *m);
void load_library(){
    // printf("dyn");
    char* path = "../zad1/libmemory.so";
    void* lib = dlopen(path,RTLD_LAZY);
    *(void **) (&init) = dlsym(lib,"init");
    *(void **) (&count) = dlsym(lib,"count");
    *(void **) (&show) = dlsym(lib,"show");
    *(void **) (&delete_all) = dlsym(lib,"delete_all");
    *(void **) (&destroy) = dlsym(lib,"destroy");
}
#else
Memory* init(int s);
void count(Memory* m,char* name);
char* show(Memory *m, int i);
void delete_all(Memory *m,int j);
void destroy(Memory *m);
#endif