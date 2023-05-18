#include<pthread.h>
#include<stdbool.h>
#include"sides.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int reindeer_count = 0;
int delivery_time = 0;
bool ended = true;
bool reached = false;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_rein = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_sant = PTHREAD_COND_INITIALIZER;
void* santa(){
    // printf("lol");
    for(int i=0;i<3;i++){
        pthread_mutex_lock(&mutex);
        while(reindeer_count<MAX_REINDEER){
            pthread_cond_wait(&cond_sant,&mutex);
        }
        pthread_mutex_unlock(&mutex);

        printf("Mikołaj: Budzę się\n");
        printf("Mikołaj: Dostarczam zabawki\n");
        usleep(delivery_time);

        pthread_mutex_lock(&mutex);
        delivery_time = 0;
        reindeer_count = 0;
        ended = false;
        printf("Mikołaj: Zasypiam\n");
        pthread_mutex_unlock(&mutex);

        pthread_cond_broadcast(&cond_rein);
    }
    exit(EXIT_SUCCESS);
    
}
void* reindeer(){
    srand(time(NULL)+pthread_self());
    while(true){
        usleep(rand()%6000000+5000000);
        ended = true;

        pthread_mutex_lock(&mutex);
        reindeer_count++;
        printf("Renifer: Czeka %d reniferów na Mikołaja,%ld\n",reindeer_count,pthread_self());
        if(reindeer_count==MAX_REINDEER){
            ended = false;
            printf("Renifer: Wybudzam Mikołaja,%ld\n",pthread_self());
            delivery_time = rand()%3000000+2000000;
            pthread_cond_broadcast(&cond_sant);
        }
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        while(ended){
            pthread_cond_wait(&cond_rein,&mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
}
void* high_elf(){
    
}