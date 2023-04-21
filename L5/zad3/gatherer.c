
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/wait.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>

double gather(int proc_num, double rec_len){
    int rec_num = (int)1.0/rec_len;
    pid_t fork_value;
    int rest;
    int rec_per_proc = (int)rec_num/proc_num;
    double first_val = 0;
    double new_start;
    double tmp;
    double sum;
    char a1[255];
    char a2[255];
    char a3[255];
    int fd;
    char buff[255];
    mkfifo("/tmp/calc",0666);
    rest = rec_num%proc_num;
    for(int i=0;i<rest;i++){
        fork_value = fork();
        if(fork_value==0){
            sprintf(a1,"%0.16lf",i*(rec_per_proc+1)*rec_len);
            sprintf(a2,"%0.16lf",rec_len);
            sprintf(a3,"%d",rec_per_proc+1);
            fd = open("/tmp/calc",O_WRONLY|O_APPEND);
            dup2(fd,STDOUT_FILENO);
            execl("./calculator","calculator",a1,a2,a3,(char *)0);
        }
    }
    new_start = rest*(rec_per_proc+1)*rec_len;
    for(int i=rest;i<proc_num;i++){
        fork_value = fork();
        if(fork_value==0){
            sprintf(a1,"%0.16lf",new_start+(i-rest)*(rec_per_proc)*rec_len);
            sprintf(a2,"%0.16lf",rec_len);
            sprintf(a3,"%d",rec_per_proc);
            fd = open("/tmp/calc",O_WRONLY|O_APPEND);
            dup2(fd,STDOUT_FILENO);
            execl("./calculator","calculator",a1,a2,a3,(char *)0);
        }
    }
    fd = open("/tmp/calc",O_RDONLY);
    for(int i=0;i<proc_num;i++){
        read(fd,&tmp,sizeof(double));
        sum += tmp;
    }
    close(fd);
    unlink("/tmp/calc");
    return sum;
}

int main(int argc, char* argv[]){
    int proc_num = atoi(argv[2]);
    double rec_len = atof(argv[1]);
    double val;
    struct timespec start;
    struct timespec end;
    double time;
    clock_gettime(CLOCK_REALTIME,&start);
    val = gather(proc_num,rec_len);
    clock_gettime(CLOCK_REALTIME,&end);
    time = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    printf("%0.16lf %0.16lf %d %lf\n",val,rec_len,proc_num,time);

    return 0;
}