#include "shm.h"
#include "consts.h"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/mman.h>




int create_shm(const char *n, int size) {
    printf("Creating a shared memory\n");
    int key = generate_key(n);
    if (key == -1) {
        perror("Unable to create shared memory");
        return key;
    }

    int shm_id = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);
    if (shm_id == -1) {
        perror("Unable to create shared memory");
        return -1;
    }
    printf("Shared memory created\n");

    return shm_id;
}

int delete_shm(const char *n) {
    int shm_id = open_shm(n);
    if (shm_id == -1) {
        return 0;
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("Unable to delete shared memory");
        return 0;
    }
    printf("Shared memory deleted");

    return 1;
}

int open_shm(const char *n) {
    int key = generate_key(n);
    if (key == -1)
        return key;
    int shm_id = shmget(key, 0, IPC_CREAT);
    if (shm_id == -1) {
        perror("Unable to open shared memory");
        return -1;
    }
    return shm_id;
}

char* attach_shm(const char *n, int size) {
    int shm_id = open_shm(n);
    if (shm_id == -1) {
        return NULL;
    }
    char *shm = shmat(shm_id, NULL, 0);
    if (shm == (char*) -1) {
        perror("Unable to attach memory");
        return NULL;
    }
    printf("Memory attached\n");
    return shm;
}

int detach_shm(char *shm) {
    if (shmdt(shm) == -1) {
        perror("Unable to detach shared memory");
        return 0;
    }
    return 1;
}

