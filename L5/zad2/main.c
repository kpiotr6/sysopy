
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
double f_val(double x){
    return 4/(x*x+1);
}


double calculate(double from,double to,double* rectangles,int rec_num,int first_rec,int last_rec){
    // int first_rec;
    // int last_rec;
    double sum = 0;
    // for(int i=0;i<rec_num;i++){
    //     if(rectangles[i]>from){
    //         first_rec = i;
    //         break;
    //     }
    // }

    // for(int i=rec_num-1;i>=0;i--){
    //     if(rectangles[i]<to){
    //         last_rec = i+1;
    //         break;
    //     }
    // }

    for(int i=first_rec+1;i<last_rec;i++){
        sum += (rectangles[i]-rectangles[i-1])*f_val((rectangles[i]+rectangles[i-1])/2);
        // printf("%f\n",sum);
    }

    if(first_rec == 0){
        sum += (rectangles[first_rec] - from)*f_val(rectangles[first_rec]/2);
        // printf("%f\n",sum);
    }
    else{
        sum += (rectangles[first_rec] - from)*f_val((rectangles[first_rec]-rectangles[first_rec-1])/2);
        // printf("%f\n",sum);
    }

    if(last_rec == rec_num - 1){
        sum += (to - rectangles[last_rec])*f_val((1+rectangles[last_rec])/2);
        // printf("%f\n",sum);
    }
    else{
        sum += (to - rectangles[last_rec])*f_val((rectangles[last_rec+1]+rectangles[last_rec])/2);
        // printf("%f\n",sum);
    }
    printf("%f\n",sum);
}

double gather(int proc_num,double rec_len){
    int rec_num = (int)1.0/rec_len;
    pid_t fork_value;
    int** pipes = malloc(sizeof(int*)*proc_num); 
    double proc_len = 1.0/proc_num;
    double *rectangles_end = malloc(sizeof(double)*rec_num);
    int j = 0;
    int k = 0;
    double from;
    double sum = 0;
    double tmp;
    for(int i=0;i<proc_num;i++){
        pipes[i] = malloc(sizeof(int)*2);
        pipe(pipes[i]);
        close(pipes[i][1]);
    }
    // int *rectangles_val = malloc(sizeof(int)*rec_num);
    for(int i=0;i<rec_num;i++){
        rectangles_end[i] = rec_len*(i+1);
    }
    for(int i=0;i<proc_num;i++){
        from = i*proc_len;
        while(rectangles_end[j]<from){
            j++;
        }
        k = j;
        while(rectangles_end[k]<from+proc_len){
            k++;
        }
        fork_value = fork();
        if(fork_value==0){
            close(pipes[i][0]);
            tmp = calculate(from,from + proc_len,rectangles_end,rec_num,j,k);
            write(pipes[i][1],&tmp,sizeof(tmp));
            exit(0);
        }
        j = k;
    }
    for(int i=0;i<proc_num;i++){
        read(pipes[i][0],&tmp,sizeof(tmp));
        sum += tmp;
    }
    return sum;
}

int main(int argc, char* argv[]){
    int proc_num = atoi(argv[2]);
    double rec_len = atof(argv[1]);
    // printf("%f",rec_len);
    printf("%f\n",gather(proc_num,rec_len));
    return 0;
}