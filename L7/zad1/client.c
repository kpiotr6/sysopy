#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "consts.h"
#include "sem.h"
#include "shm.h"
#include "queue.h"




int main(int argc, char **argv) {
    srand(getpid()+time(NULL));
    setbuf(stdout, NULL);
    printf("[Client %d] Created\n", getpid());
    int sem_c = open_sem(CHAI_SEM);
    int sem_q = open_sem(QUEU_SEM);
    int sem_h = open_sem(HAIR_SEM);
    char *q = attach_shm(QUEU_NAME, QEUE_MAX);
    if (q == NULL)
        return -1;
    if (q_is_full(q)) {
        printf("(Client %d) Queue is full\n", getpid());
        return 0;
    }
    block(sem_q);
    char haircut = (char) (rand() % 30);
    q_push(q, haircut);
    unblock(sem_c);
    unblock(sem_h);
    detach_shm(q);
    return 0;
}

