#include<pthread.h>
#include<stdbool.h>
#include"sides.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int elf_count = 0;
bool im_free[] = {false,false,false};
int help_time[] = {0,0,0};

int reindeer_count = 0;
int delivery_time = 0;

bool ended = true;
bool reached = false;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_rein = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_sant = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_elf = PTHREAD_COND_INITIALIZER;

void* santa(){
    for(int i=0;i<3;i++){
        pthread_mutex_lock(&mutex);
        while(reindeer_count<MAX_REINDEER && elf_count<3){
            pthread_cond_wait(&cond_sant,&mutex);
        }
        printf("Mikołaj: Budzę się\n");

        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex);
        if(elf_count==3){
            pthread_mutex_unlock(&mutex);
            for(int i=0;i<3;i++){
                pthread_mutex_lock(&mutex);
                im_free[0] = false;
                im_free[1] = false;
                im_free[2] = false;
                im_free[i] = true;
                pthread_mutex_unlock(&mutex);
                pthread_cond_broadcast(&cond_elf);
                usleep(help_time[i]);
            }

        }
        else{
            printf("Mikołaj: Dostarczam zabawki\n");
            usleep(delivery_time);
        }
        pthread_mutex_lock(&mutex);
        delivery_time = 0;
        reindeer_count = 0;
        for(int i=0;i<3;i++){
            im_free[i] = true;
            help_time[i] = 0;
        }
        elf_count = 0;
        ended = false;
        printf("Mikołaj: Zasypiam\n");
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond_elf);
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
    int i;
    srand(time(NULL)+pthread_self());
    while(true){
        usleep(rand()%6000000+5000000);
        pthread_mutex_lock(&mutex);
        if(elf_count<3){
            i = elf_count;
            elf_count++;
            help_time[i] = rand()%1000000+2000000;
            printf("Elf: Czeka %d elfów na Mikołaja,%ld\n",elf_count,pthread_self());
            if(i == 2){
                printf("Elf: Wybudzam Mikołaja%ld\n",pthread_self());
                pthread_cond_broadcast(&cond_sant);
            }
            while(!im_free[i]){
                pthread_cond_wait(&cond_elf,&mutex);
            }
            printf("Elf: Mikołaj rozwiązuje problem,%ld\n",pthread_self());

            // while(!im_free[i]){
            //     pthread_cond_wait(&cond_elf,&mutex);
            // }
            pthread_mutex_unlock(&mutex);
            pthread_cond_broadcast(&cond_sant);
        }
        else{
            printf("Elf: Samodzielnie rozwiązuje swój problem,%ld\n",pthread_self());
            pthread_mutex_unlock(&mutex);
            usleep(rand()%1000000+2000000);
        }

    }
}