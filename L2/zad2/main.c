#include <stdio.h>
#include <time.h>
#include <string.h>

#define BUFFER 1024

void reverse_one(char* from,char* to){
    FILE* f = fopen(from,"rb");
    FILE* t = fopen(to,"wb");
    char c[1];
    fseek(f,1,SEEK_END);
    while(!fseek(f,-2,SEEK_CUR)){
        fread(c,sizeof(char),1,f);
        fwrite(c,sizeof(char),1,t);
    }
    fclose(f);
    fclose(t);
}

void flip(char* to_flip,int size){
    char b;
    for(int i=0;i<size/2;i++){
        b = to_flip[i];
        to_flip[i] = to_flip[size-i-1];
        to_flip[size-i-1] = b;
    }
}


void reverse_buff(char* from,char* to){
    FILE* f = fopen(from,"rb");
    FILE* t = fopen(to,"wb");
    char buff[BUFFER];
    long rest;

    fseek(f,0,SEEK_END);
    while(!fseek(f,-BUFFER,SEEK_CUR)){
        fread(buff,sizeof(char),BUFFER,f);
        flip(buff,BUFFER);
        fwrite(buff,sizeof(char),BUFFER,t);
        fseek(f,-BUFFER,SEEK_CUR);
    }
    rest = ftell(f);
    if(rest>0){
        fseek(f,0,SEEK_SET);
        fread(buff,sizeof(char),rest,f);
        flip(buff,rest);
        fwrite(buff,sizeof(char),rest,t);
    }
    fclose(f);
    fclose(t);
}

int main(int argc,char *argv[]){
    clock_t o,b;
    o = clock();
    reverse_one(argv[1],argv[2]);
    printf("One CPU time: %ld\n",clock()-o);
    b = clock();
    reverse_buff(argv[1],strcat(argv[2],"_buff"));
    printf("Buffer CPU time: %ld\n",clock()-b);
    // printf("%f",3/5);
    return 0;
}