#include<stdio.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include <sys/wait.h>

void handle_fun(int sig){
    printf("Obsluga sygnalu SIGUSR1\n");
}

void ignore(){
    signal(SIGUSR1,SIG_IGN);
}

void handler(){
    signal(SIGUSR1,handle_fun);
}

void mask(){
    sigset_t block;
    sigaddset(&block,SIGUSR1);
    sigprocmask(SIG_SETMASK,&block,NULL);
}


int main(int argc,char *argv[]){
    sigset_t pending;
    if(argc!=2){
        fprintf(stderr,"Wrong number of arguments\n");
        return 1;
    }
    if(strcmp(argv[1],"ignore")==0){
        ignore();
        raise(SIGUSR1);
    }
    else if(strcmp(argv[1],"handler")==0){
        handler();
        raise(SIGUSR1);
    }
    else if(strcmp(argv[1],"mask")==0){
        mask();
        raise(SIGUSR1);
    }
    else if(strcmp(argv[1],"pending")==0){
        mask();
        raise(SIGUSR1);
        sigpending(&pending);
        if(sigismember(&pending,SIGUSR1)){
            printf("SIGUSR1 pending\n");
        }
    }

    int fvalue = fork();
    if(fvalue!=0){
        execl("./loop","loop",(char *)0);
    }
    kill(getppid(),SIGUSR1);
    return 0;
}