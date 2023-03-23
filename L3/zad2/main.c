#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
    int fvalue;
   
    if(argc!=2){
        perror("Wrong number of arguments");
        return 1;
    }
    printf("%s\n",argv[0]);
    fflush(stdout);
    execl("/bin/ls","-l",argv[1],(char *)0);
    return 0;
}