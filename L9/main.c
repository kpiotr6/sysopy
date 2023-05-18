#include "sides.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    pthread_t tid;
    setbuf(stdout, NULL);
    for(int i=0;i<9;i++){
        pthread_create(&tid,NULL,reindeer,NULL);
    }
    pthread_create(&tid,NULL,santa,NULL);
    while(1){
        sleep(10);
    }
    return 0;
}