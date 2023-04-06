
#include<stdio.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int ctr = 0;

int pid;

int* commands;


int max;

void handler(int sig){
    if(ctr==max){
        exit(0);
    }
    union sigval val;
    val.sival_int = commands[ctr];
    sigqueue(pid,SIGUSR1,val);
    ctr++;
}

int main(int argc, char *argv[]){
    union sigval val;
    if(argc<3){
        fprintf(stderr,"Wrong number of arguments\n");
        return 1;
    }
    pid = atoi(argv[1]);
    max = argc-2;
    // printf("%d\n",max);
    commands = malloc(sizeof(int)*(max));
    for(int i=2;i<argc;i++){
        commands[i-2] = atoi(argv[i]);
    }
    val.sival_int = commands[0];
    ctr++;
    // printf("%d\n",ctr);
    signal(SIGUSR1,handler);
    sigqueue(pid,SIGUSR1,val);
    while(1){
        sleep(1);
    }
}