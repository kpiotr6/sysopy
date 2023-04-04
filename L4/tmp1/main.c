#include <stdio.h>
#include <signal.h>

void signal_print(int signal){
    printf("ja cie ale sygnal\n");
}

int main(int argc,char *argv[]){
    signal(SIGINT,signal_print);
    raise(SIGINT);
    return 0;
}