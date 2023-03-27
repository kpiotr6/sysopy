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
    printf("%s",argv[0]);
    fflush(stdout);
    execl("/bin/ls","ls",argv[1],(char *)0);
    return 0;
}