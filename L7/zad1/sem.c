
#include <string.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <stdio.h>
#include "consts.h"
#include "sem.h"
#include <sys/types.h>

typedef struct sembuf Sembuf;

void unblock(int sem_id) {
    Sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;
    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("Unable to unblock semaphore");
        return;
    }
}

void block(int sem_id) {
    Sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;
    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("Unable to block semaphore");
        return;
    }
}


int generate_key(const char* n){
    int key = ftok(HOME, n[0]);
    if (key == -1) {
        perror("Unable to create key");
        return -1;
    }
    return key;
}

int new_sem(const char* n, int st_val) {
    printf("Semaphore is being created\n");
    int key = generate_key(n);
    if(key == -1){
        return -1;
    }
    int sem_id = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (sem_id == -1) {
        perror("Unable to create semaphore");
        return -1;
    }
    if (semctl(sem_id, 0, SETVAL, st_val) == -1) {
        perror("Unable to set value to semaphore");
        return -1;
    }
    printf("Semaphore creation finished\n");
    return sem_id;
}

int open_sem(const char* n) {
    int key = generate_key(n);
    if(key == -1){
        return -1;
    }
    int sem_id = semget(key, 1, IPC_CREAT);
    if (sem_id == -1) {
        perror("Unable to open semaphore");
        return -1;
    }

    return sem_id;
}



int del_sem(const char *n) {
    int sem_id = open_sem(n);
    if (sem_id == -1)
        return 0;

    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Unable to delete semaphore");
        return 0;
    }
    printf("Semaphore deleted\n");
    return 1;
}
