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
    act.sa_handler = handler;
    act.sa_flags = SA_NOCLDSTOP;

    //Flaga SA_NOCLDSTOP sprawia, że blokowane są sygnały SIGCHLD wysyłane przez dziecko gdy otrzyma
    //ono sygnały SIGSTOP, SIGCONT.

    sigaction(SIGCHLD,&act,NULL);
    int fvalue = fork();
    if(fvalue==0){
        raise(SIGSTOP);
        printf("Proces Potomny\n");
        sleep(1);
        return 0;
    }
    sleep(1);
    sleep(1);
    if(fvalue!=0){
        kill(fvalue,SIGCONT);
    }
    sleep(2);
    printf("\n");

    //SA_SIGINFO

    sigset_t blocked;
    sigfillset(&blocked);
    act.sa_mask = blocked;
    act.sa_sigaction = handler_advanced;
    act.sa_flags = SA_SIGINFO;

    //Flaga SA_SIGINFO sprawia, że jako hanlder otrzymanego sygnału zostanie użyta funckja
    //przekazana za pomocą act.sa_sigaction do której przesyłane są dodatkowe argumenty

    sigaction(SIGUSR1,&act,NULL);
    raise(SIGUSR1);

    //SA_RESETHAND
    act.sa_handler = handler;
    act.sa_flags = SA_RESETHAND;
    //Flaga SA_RESETHAND sprawia, że po jednokrotnym złapaniu sygnału, działanie tego sygnału
    //powraca do stanu domyślnego 
    sigaction(SIGUSR1,&act,NULL);
    raise(SIGUSR1);
    raise(SIGUSR1);
    return 0;
}