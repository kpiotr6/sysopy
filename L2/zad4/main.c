#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <limits.h>
typedef struct stat stat_t;
typedef struct dirent dirent_t;

long long sum = 0;

int check(const char* path,const stat_t* file_stat,int type){
    if(type!=FTW_D){
        printf("%15ld %15s\n",file_stat->st_size,path);
        sum += file_stat->st_size;
    }
    return 0;

}

int (*check_fun)(const char* ,const stat_t* ,int) = check;

int main(int argc,char* argv[]){
    ftw(argv[1],check_fun,10);
    printf("Sumaryczny rozmiar: %lld\n",sum);
}