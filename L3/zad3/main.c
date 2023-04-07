#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h>

#define BUFFER 255

char error[BUFFER+PATH_MAX];

int mstarts_with(char* file_path,char* pattern){
    char buffer[BUFFER];
    FILE* file_handle;
    file_handle = fopen(file_path,"r");
    if(!file_handle){
        sprintf(error,"Unable to open file %s\n",file_path);
        perror(error);
        errno = 0;
        return -1;
    }
    fread(buffer,sizeof(char),BUFFER,file_handle);
    if(ferror(file_handle)){
        sprintf(error,"Unable to read file %s\n",file_path);
        perror(error);
        return -1;
    }
    if(fclose(file_handle)){
        sprintf(error,"Unable to close file %s\n",file_path);
        perror(error);
        errno = 0;
        return -1;
    }
    buffer[strlen(pattern)]='\0';
    if(!strcmp(pattern,buffer)){
        return 1;
    }
    return 0;
}

void find(char* path,char* pattern){
    struct dirent* file;
    struct stat file_stat;
    char file_path[PATH_MAX];
    DIR* current = opendir(path);
    int fork_value;
    if(!current){
        sprintf(error,"Unable to open folder %s\n",path);
        perror(error);
        errno = 0;
        exit(-1);
    }
    while(file = readdir(current)){
        if(!strcmp(file->d_name,".") || !strcmp(file->d_name,"..")){
            continue;
        }
        if(path[strlen(path)-1]=='/'){
            sprintf(file_path,"%s%s",path,file->d_name);
        }
        else{
            sprintf(file_path,"%s/%s",path,file->d_name);
        }

        if(stat(file_path,&file_stat)){
            sprintf(error,"Unable get information about file %s\n",file_path);
            perror(error);
            errno = 0;
        }
        if(S_ISDIR(file_stat.st_mode)){
            fork_value = fork();
            if(fork_value==0){
                find(file_path,pattern);
                return;
            }
            if(fork_value==-1){
                sprintf(error,"Unable to create new process for folder %s\n",file_path);
                perror(error);
                errno = 0;
                continue;
            }
        }
        if(S_ISREG(file_stat.st_mode)){
            if(starts_with(file_path,pattern)==1){
                printf("%s %d\n",file_path,getpid());
            }
        }
    }
    if(file){
        sprintf(error,"Unable to read folder %s\n",path);
        perror(error);
        errno = 0;
    }
    if(closedir(current)){
        sprintf(error,"Unable to read folder %s\n",path);
        perror(error);
        errno = 0;
    }

    while(wait(NULL)>0){}
    return;
}


int main(int argc,char *argv[]){
    if(argc!=3){
        sprintf(error,"Wrong number of arguments");
        perror(error);
        exit(-1);
    }
    find(argv[1],argv[2]);
    return 0;
}