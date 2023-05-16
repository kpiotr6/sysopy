#include<pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void* th_hello(void* n){
    int* v = malloc(sizeof(int));
    *v = *(int*)n;
    sleep(5);
    return (void*)v;
}
int a = 30;

int main(){
    pthread_t tid[10];
    void *rval;
    for(int i=0;i<10;i++){
        pthread_create(tid+i,NULL,th_hello,&a);
        a++;
    }
    for(int i=0;i<10;i++){
        pthread_join(tid[i],&rval);
        printf("%d\n",*((int*)rval));
    }
    sleep(2);  
    printf("Hello\n");
    printf("Hello\n");

    return 0;
}