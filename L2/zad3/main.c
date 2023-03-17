#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct stat stat_t;
typedef struct dirent dirent_t;

int main(){
    
    DIR* curr = opendir(".");
    dirent_t* file;
    stat_t file_stat;
    long long sum = 0;
    while(file = readdir(curr)){
        stat(file->d_name,&file_stat);
        if(S_ISDIR(file_stat.st_mode)){
            continue;
        }
        if(S_ISLNK(file_stat.st_mode)){
            lstat(file->d_name,&file_stat);
        }
        sum += file_stat.st_size;
        printf("%15ld %15s\n",file_stat.st_size,file->d_name);
    }
    printf("Sumaryczny rozmiar: %lld\n",sum);
    closedir(curr);
}