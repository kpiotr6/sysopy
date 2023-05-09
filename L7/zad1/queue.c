#include "queue.h"
#include "consts.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>


int q_is_full(char *q) {
    return strlen(q) >= QEUE_MAX;
}

int q_is_empty(char *q) {
    return strlen(q) == 0;
}

char q_pop(char *q) {
    if(q_is_empty(q)) {
        errno = 22;
        perror("Queue is empty.\n");
        return '\0';
    }
    char n = q[0];
    memmove(q, q + 1, strlen(q) + 1);
    return n;
}

void q_push(char *q, char n) {
    if(q_is_full(q)) {
        errno = 22;
        perror("Queue is full.\n");
        return;
    }
    int s = strlen(q);
    q[s] = n;
    q[s+1] = '\0';
}

