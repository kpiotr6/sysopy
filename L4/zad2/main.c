#include<stdio.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>

//SA_NODEFER SA_RESETHAND SA_SIGINFO

void handler_advanced(int sig,siginfo_t* info,void* context){
    printf("Advanced\nSignal: %d\nPID: %d\n\n",sig,info->si_pid);
    return;
}

void handler(int sig){
    printf("Standard\n");
    return;
}

int main(int argc,char *argv[]){
    struct sigaction act;

    //SA_NOCLDSTOP
    int fvalue = fork();
    printf("%d\n",fvalue);
    if(fvalue==0){
        sleep(2);
        raise(SIGSTOP);
        return 0;
    }
    act.sa_handler = handler;
    act.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD,&act,NULL);
    kill(fvalue,SIGINT);
    
    //SA_SIGINFO
    sigset_t blocked;
    sigfillset(&blocked);
    act.sa_mask = blocked;
    act.sa_sigaction = handler_advanced;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);
    raise(SIGUSR1);

    //SA_RESETHAND
    act.sa_handler = handler;
    act.sa_flags = SA_RESETHAND;
    
    sigaction(SIGUSR1,&act,NULL);
    raise(SIGUSR1);
    raise(SIGUSR1);
    return 0;
}