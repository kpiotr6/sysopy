
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/wait.h>
#include<time.h>

double double f_val( double x){
    return 4/(x*x+1);
}


 double calculate( double* rectangles,int first_rec,int rec_number, double rec_len){
     double sum = 0;
    for(int i=first_rec;i<first_rec+rec_number;i++){
        sum += f_val((rectangles[i]+rectangles[i-1])/2)*rec_len;
    }
    return sum;
}

 double gather(int proc_num, double rec_len){
    int rec_num = (int)1.0/rec_len;
    pid_t fork_value;
    int** pipes = malloc(sizeof(int*)*proc_num); 
     double *rectangles_end = malloc(sizeof( double)*rec_num);
     double sum = 0;
     double tmp;
    int from;
    int start;
    int rest;
    int rec_per_proc = (int)rec_num/proc_num;
     double first_val = 0;
    for(int i=0;i<proc_num;i++){
        pipes[i] = malloc(sizeof(int)*2);
        pipe(pipes[i]);
    }
    for(int i=0;i<rec_num;i++){
        rectangles_end[i] = rec_len*(i+1);
    }
    rest = rec_num%proc_num+1;
    fork_value = fork();
    if(fork_value==0){
        for(int i=0;i<rec_per_proc+1;i++){
            first_val += f_val((rectangles_end[i]+rectangles_end[i-1])/2)*rec_len;
        }
        write(pipes[0][1],&first_val,sizeof( double));
        exit(0);
    }

    for(int i=1;i<rest;i++){
        fork_value = fork();
        if(fork_value==0){
            tmp = calculate(rectangles_end,i*(rec_per_proc+1),rec_per_proc+1,rec_len);
            write(pipes[i][1],&tmp,sizeof( double));
            exit(0);
        }
    }

    from = rest*(rec_per_proc+1);

    for(int i=rest;i<proc_num;i++){
        fork_value = fork();
        if(fork_value==0){
            tmp = calculate(rectangles_end,from+(i-rest)*rec_per_proc,rec_per_proc,rec_len);
            write(pipes[i][1],&tmp,sizeof( double));
            exit(0);
        }
    }
    for(int i=0;i<proc_num;i++){
        
        read(pipes[i][0],&tmp,sizeof( double));
        sum += tmp;
    }
    free(rectangles_end);
    for(int i=0;i<proc_num;i++){
        free(pipes[i]);
    }
    free(pipes);
    return sum;
}

int main(int argc, char* argv[]){
    int proc_num = atoi(argv[2]);
     double rec_len = atof(argv[1]);
     double val;
    clock_t t = clock();
    val = gather(proc_num,rec_len);
    printf("%0.8f %0.8f %ld\n",val,rec_len,clock()-t);
    return 0;
}