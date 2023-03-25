#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
    int fvalue;
    if(argc!=2){
        fprintf( stderr, "Wrong number of arguments\n");
        return -1;
    }
    for(int i = 0;i<atoi(argv[1]);i++){
        fvalue = fork();
        if(fvalue==0){
            printf("%d %d\n",getppid(),getpid());
            exit(1);
        }
    }
    while(wait(NULL) > 0){}
    printf("%s\n",argv[1]);
    return 0;
}