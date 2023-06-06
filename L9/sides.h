#ifndef SIDES
#define SIDES
#include<pthread.h>
#define MAX_REINDEER 9

void* santa();
void* reindeer();
void* high_elf();

typedef enum woke_by{
    HIGH_ELF,
    REINDEER,
    NONE
} WokeBy;

#endif