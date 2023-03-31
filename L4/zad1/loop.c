#include <unistd.h>
#include <stdio.h>
#include<signal.h>


int main(int argc,char *argv[]){
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending,SIGUSR1)){
        printf("SIGUSR1 pending exec\n");
    }
    // printf("lol");
    sleep(1);
    // fflush(stdin);
}