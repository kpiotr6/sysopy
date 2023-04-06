
#include<stdio.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int ctr = 0;

int is_interrupted = 0;

void handler(int sig,siginfo_t* info,void* context){
    time_t t;
    struct tm* real;
    sigset_t set;
    ctr++;
    if(info->si_value.sival_int!=4){
        is_interrupted = 1;
    }
    kill(info->si_pid,SIGUSR1);
    switch(info->si_value.sival_int)
    {
        case 1:
            for(int i=1;i<101;i++){
                printf("%d ",i);
            }
            printf("\n");
            break;
        case 2:
            time(&t);
            real = localtime(&t);
            printf("%d-%02d-%02d\n",real->tm_year+1900,real->tm_mon+1,real->tm_mday);
            break;
        case 3:
            printf("%d\n",ctr);
            break;
        case 4:
            is_interrupted = 0;
            while(!is_interrupted){
                time(&t);
                real = localtime(&t);
                printf("%d-%02d-%02d\n",real->tm_year+1900,real->tm_mon+1,real->tm_mday);
                sigpending(&set);
                sleep(1);
            }
            break;
        case 5:
            exit(0);
            break;                                    
    }
    // is_interrupted = 0;
}

int main(int argc, char *argv[]){
    struct sigaction act;
    sigset_t block;
    sigemptyset(&block);
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO|SA_NODEFER;
    act.sa_mask = block;
    sigaction(SIGUSR1,&act,NULL);
    printf("%d\n",getpid());
    while(1){
        sigsuspend(&block);
    }

}