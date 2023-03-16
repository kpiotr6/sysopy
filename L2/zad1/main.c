#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>


#define BUFFER 1024

#ifdef SYS
void rewrite(char* find,char* change,char* from,char* to){
    char buff[1024];
    int from_fd = open(from,O_RDONLY);
    int to_fd = open(to,O_WRONLY|O_CREAT);
    int read_ctr;
    while(read_ctr = read(from_fd,buff,BUFFER)){
        for(int i=0;i<strlen(buff);i++){
            if(buff[i]==*find){
                buff[i]=*change;
            }
        }
        write(to_fd,buff,read_ctr);
    }

    close(from_fd);
    close(to_fd);
    chmod(to,S_IRWXU|S_IRWXG|S_IRWXO);
}
#else
void rewrite(char* find,char* change,char* from,char* to){
    char buff[1024];
    FILE* from_fd = fopen(from,"r");
    FILE* to_fd = fopen(to,"w");
    int read_ctr;
    while(read_ctr = fread(buff,sizeof(char),BUFFER,from_fd)){
        for(int i=0;i<strlen(buff);i++){
            if(buff[i]==*find){
                buff[i]=*change;
            }
        }
        fwrite(buff,sizeof(char),read_ctr,to_fd);
    }
    fclose(from_fd);
    fclose(to_fd);
    chmod(to,S_IRWXU|S_IRWXG|S_IRWXO);
}
#endif

int main(int argc,char *argv[]){
    if(argc<4){
        fprintf(stderr,"Not enough arguments.\n");
        return 1;
    }
    if(strlen(argv[1])>1 || strlen(argv[2])>1){
        fprintf(stderr,"Arg1 and Arg2 arent single characters.\n");
        return 1;
    }
    clock_t p = clock();  
    rewrite(argv[1],argv[2],argv[3],argv[4]);
    #ifdef SYS
    printf("SYS CPU time: %ld\n",clock()-p);
    #else
    printf("LIB CPU time: %ld\n",clock()-p);
    #endif

    return 0;
}