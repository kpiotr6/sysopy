#include<pthread.h>
#include<stdbool.h>
#include"sides.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int elf_count = 0;
int help_time[] = {0,0,0};
bool im_free[] = {false,false,false};

int reindeer_count = 0;
int delivery_time = 0;
bool ended = true;

bool santa_sleep = true;
WokeBy by = NONE;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_rein = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_sant = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_elf = PTHREAD_COND_INITIALIZER;

int max(int a, int b){
    if(a<b){
        a = b;
    }
    return a;
}

void* santa(){
    int mctr = 0;
    while(mctr<3){
        pthread_mutex_lock(&mutex);
        while(by==NONE){
            pthread_cond_wait(&cond_sant,&mutex);
        }

        printf("Mikołaj: Budzę się\n");
        santa_sleep = false;
        if(by==HIGH_ELF){
            for(int i=0;i<3;i++){
                im_free[max(i-1,0)] = false;
                im_free[i] = true;
                pthread_mutex_unlock(&mutex);

                pthread_cond_broadcast(&cond_elf);
                usleep(help_time[i]);
                
                pthread_mutex_lock(&mutex);
            }            

            for(int i=0;i<3;i++){
                im_free[i] = false;
                help_time[i] = 0;
            }
            elf_count = 0;
        }
        else{
            printf("Mikołaj: Dostarczam zabawki\n");
            pthread_mutex_unlock(&mutex);

            usleep(delivery_time);

            pthread_mutex_lock(&mutex);
            delivery_time = 0;
            reindeer_count = 0;
            ended = false;
            mctr++;
        }
        santa_sleep = true;
        by = NONE;
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

        pthread_mutex_lock(&mutex);
        ended = true;
        reindeer_count++;
        printf("Renifer: Czeka %d reniferów na Mikołaja,%ld\n",reindeer_count,pthread_self());
        if(reindeer_count==MAX_REINDEER){
            while(!santa_sleep){
                pthread_cond_wait(&cond_rein,&mutex);
            }

            ended = false;
            printf("Renifer: Wybudzam Mikołaja,%ld\n",pthread_self());
            by = REINDEER;
            delivery_time = rand()%3000000+2000000;

            pthread_cond_broadcast(&cond_sant);
        }
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
        usleep(rand()%9000000+5000000);

        pthread_mutex_lock(&mutex);
        if(elf_count<3){
            i = elf_count;
            elf_count++;
            help_time[i] = rand()%400000+2000000;
            printf("Elf: Czeka %d elfów na Mikołaja,%ld\n",elf_count,pthread_self());
            if(i == 2){
                while(!santa_sleep){
                    pthread_cond_wait(&cond_elf,&mutex);
                }
                by = HIGH_ELF;
                printf("Elf: Wybudzam Mikołaja%ld\n",pthread_self());

                pthread_cond_broadcast(&cond_sant);
            }
            while(!im_free[i]){
                pthread_cond_wait(&cond_elf,&mutex);
            }

            printf("Elf: Mikołaj rozwiązuje problem,%ld\n",pthread_self());
            pthread_mutex_unlock(&mutex);
        }
        else{
            printf("Elf: Samodzielnie rozwiązuje swój problem,%ld\n",pthread_self());
            pthread_mutex_unlock(&mutex);

            usleep(rand()%1000000+2000000);
        }

    }
}