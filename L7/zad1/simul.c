
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include "consts.h"
#include "sem.h"
#include "shm.h"

int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    printf("Start of simulation\n");
    int sem_h = new_sem(HAIR_SEM, HAIR_MAX);
    int sem_c = new_sem(CHAI_SEM, CHAI_MAX);
    int sem_q = new_sem(QUEU_SEM, QEUE_MAX);
    create_shm(QUEU_NAME, QEUE_MAX + 1);
    char *queue = attach_shm(QUEU_NAME, QEUE_MAX);
    if (queue == NULL){
        return -1;
    } 
    for (int i = 0; i < CLIE_MAX; i++) {
        if (fork() == 0) {
            execl(CLIE_PATH, CLIE_PATH, (char *) NULL);
        }
    }
    for (int i = 0; i < HAIR_MAX; i++) {
        if (fork() == 0) {
            execl(HAIR_PATH, HAIR_PATH, (char *) NULL);
        }
    }
    while(wait(NULL) > 0);
    detach_shm(queue);
    delete_shm(QUEU_NAME);
    del_sem(QUEU_SEM);
    del_sem(HAIR_SEM);
    del_sem(CHAI_SEM);
    printf("\nEnd of simuation\n");
    return 0;
}

