#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <prerror.h>
#include <stdio.h>
#include <sys/types.h>
#define BUFF 1024

typedef union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
} Semun;


int create_semaphore(int id,int num){
    char* HOME = getenv("HOME");
    int key = ftok(HOME,id);
    if(key == -1){
        perror("Unable to create key");
        return -1;
    }
    int sem_id = semget(key,num,IPC_CREAT|IPC_EXCL|0x666);
    if(sem_id == -1){
        perror("Unable to create semaphores");
        return -1;
    }
    return sem_id;
}

void set_sem_values(int sem_id,int* values,int sem_num){
    Semun sem_set;
    char err_msg[BUFF];
    for (int i = 0; i < sem_num; i++)
    {
        sem_set.val = values[i];
        if(semctl(sem_id,i,IPC_SET,sem_set) == -1){
            sprintf(err_msg,"%s %d","Unable to set value for semaphore ",i);
            perror(err_msg);
        }    
    }
}