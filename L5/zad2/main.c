
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/wait.h>
#include<time.h>

double f_val( double x){
    return 4/(x*x+1);
}


 double calculate(double from, double width, int number){
    double sum = 0;
    for(int i=0;i<number;i++){
        sum += f_val(from+i*width+width/2)*width;
    }
    return sum;
}

double gather(int proc_num, double rec_len){
    int rec_num = (int)1.0/rec_len;
    pid_t fork_value;
    int** pipes = malloc(sizeof(int*)*proc_num); 
    int rest;
    int rec_per_proc = (int)rec_num/proc_num;
    double first_val = 0;
    double new_start;
    double tmp;
    double sum;
    for(int i=0;i<proc_num;i++){
        pipes[i] = malloc(sizeof(int)*2);
        pipe(pipes[i]);
    }
    rest = rec_num%proc_num;
    for(int i=0;i<rest;i++){
        fork_value = fork();
        if(fork_value==0){
            tmp = calculate(i*(rec_per_proc+1)*rec_len,rec_len,rec_per_proc+1);
            write(pipes[i][1],&tmp,sizeof( double));
            exit(0);
        }
    }
    new_start = rest*(rec_per_proc+1)*rec_len;
    for(int i=rest;i<proc_num;i++){
        fork_value = fork();
        if(fork_value==0){
            tmp = calculate(new_start+(i-rest)*(rec_per_proc)*rec_len,rec_len,rec_per_proc);
            write(pipes[i][1],&tmp,sizeof(double));
            exit(0);
        }

    }
    for(int i=0;i<proc_num;i++){
        read(pipes[i][0],&tmp,sizeof( double));
        sum += tmp;
    }
    free(pipes);
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