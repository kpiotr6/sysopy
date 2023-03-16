#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#define BUFFER 1024

void rewrite(char* find,char* change,char* from,char* to){
    char buff[1024];
    int from_fd = open(from,O_RDONLY);
    int to_fd = open(to,O_WRONLY|O_CREAT);
    int read_ctr;
    while(read_ctr = read(from_fd,buff,BUFFER)){
        for(int i=0;i<BUFFER;i++){
            
        }
        write(to_fd,buff,read_ctr);
    }
    chmod(to,S_IRWXU|S_IRWXG|S_IRWXO);
}

int main(int argc,char *argv[]){
    if(argc<4){
        fprintf(stderr,"Not enough arguments.\n");
        return 1;
    }
    if(strlen(argv[1])>1 || strlen(argv[2])>1){
        fprintf(stderr,"Arg1 and Arg2 arent single characters.\n");
        return 1;
    }  
    rewrite(argv[1],argv[2],argv[3],argv[4]); 
    return 0;
}