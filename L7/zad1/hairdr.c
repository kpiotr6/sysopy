
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "consts.h"
#include "sem.h"
#include "shm.h"
#include "queue.h"

#define TIME_BASE 250000

int main(int argc, char **argv) {
    srand(time(NULL));
    setbuf(stdout, NULL);
    printf("(Hairdresser %d) Created\n", getpid());
    int sem_c = open_sem(CHAI_SEM);
    int sem_q = open_sem(QUEU_SEM);
    int sem_h = open_sem(HAIR_SEM);
    char *queue = attach_shm(QUEU_NAME, QEUE_MAX);
    if (queue == NULL)
        return -1;
    while (1) {
        block(sem_h);
        char hair_time = q_pop(queue);
        printf("(Hairdresser %d) Haircut: %hhd Time: %.2f s\n",getpid(), hair_time, hair_time * TIME_BASE / 1000000.0);
        usleep(hair_time * TIME_BASE);
        printf("(Hairdresser %d) Finished haircut%hhd\n", getpid(), hair_time);
        unblock(sem_c);
        unblock(sem_q);
        if (q_is_empty(queue)) {
            sleep(1);
            if (q_is_empty(queue))
                break;
        }
    }
    printf("(Hairdresser %d) Customer served\n", getpid());
    detach_shm(queue);
    return 0;
}
