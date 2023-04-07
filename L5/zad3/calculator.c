
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/wait.h>
#include<time.h>

double f_val( double x){
    return 4/(x*x+1);
}

int main(int argc, char* argv[]){
    double from = atof(argv[1]);
    double width = atof(argv[2]);
    int number = atoi(argv[3]);
    double sum = 0;
    for(int i=0;i<number;i++){
        sum += f_val(from+i*width+width/2)*width;
    }
    // fprintf(stderr,"%0.16f %0.16f %d \n",from,width,number);
    // fprintf(stderr,"%s %s %s \n",argv[1],argv[2],argv[3]);
    // write(STDERR_FILENO,&sum,sizeof(double));
    write(STDOUT_FILENO,&sum,sizeof(double));
    // printf("%f",sum);
    return 0;
}