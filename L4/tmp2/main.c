#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int i = 0;

void signal_print(int signal){
    i++;
    printf("%d\n",i);
}

int main(int argc,char *argv[]){
    
    int fval = fork();
    printf("%d\n",fval==0);
    if(fval==0){
        signal(SIGUSR1,signal_print);
        // while(1){
        //     sleep(100);
        // }
        
    }
    else{
        printf("k");
        while(1){
            kill(fval,SIGUSR1);
            sleep(100);
        }
    }
    return 0;
}
