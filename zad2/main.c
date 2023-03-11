#include<string.h>
#include<stdio.h>
#include<regex.h>
#include<time.h>
#include<sys/times.h>
#include "../zad1/memory.h"
#define COMMAND_NUM 7


typedef enum{
    INIT=1,
    COUNT=2,
    SHOW=3,
    DELETE=4,
    DESTROY=5,
    END=6,
    ERROR=7
} Option;

Option  commands[] = {
    INIT,
    COUNT,
    SHOW,
    DELETE,
    DESTROY,
    END,
    ERROR
};

Memory* m;
int exists = 0;



regex_t* get_regex(){
    regex_t *reg = calloc(COMMAND_NUM,sizeof(regex_t));
    regcomp(reg,"^ *\t*INIT *\t*[1-9][0-9]* *\t*\n$",REG_ICASE);
    regcomp(reg+1,"^ *\t*COUNT *\t*.* *\t*\n$",REG_ICASE);
    regcomp(reg+2,"^ *\t*SHOW *\t*[0-9][0-9]* *\t*\n$",REG_ICASE);
    regcomp(reg+3,"^ *\t*DELETE INDEX *\t*[0-9][0-9]* *\t*\n$",REG_ICASE);
    regcomp(reg+4,"^ *\t*DESTROY *\t*\n$",REG_ICASE);
    regcomp(reg+5,"^ *\t*END *\t*\n$",REG_ICASE);
    return reg;
}

Option get_command(const char *line,regex_t *reg){
    for(int i=0;i<COMMAND_NUM-1;i++){
        if(!regexec(reg+i,line,0,NULL,0)){
            return commands[i];
        }
    }
    return ERROR;
}
char* get_nth_word(char* line, int n){

    int i = 0;
    char* last;
    char* word = strtok(line," \t\n");
    while(word){
        i++;
        if(i==n){
            last = word;
        }
        word = strtok(NULL," \t\n");
    }
    return last;
}

char* exe_command(Option command, char* line){
    int val;
    char* word;
    if(!exists && command!=INIT){
        fprintf(stderr,"Memory is not allocated. Use INIT\n");
    }
    else{
        switch (command)
            {
            case INIT:
                if(exists){
                    destroy(m);
                }
                val = atoi(get_nth_word(line,2));
                m = init(val);
                exists = 1;
                break;
            case COUNT:
                count(m,get_nth_word(line,2));
                break;
            case SHOW:
                val = atoi(get_nth_word(line,2));
                return show(m,val);
                break;
            case DELETE:
                val = atoi(get_nth_word(line,3));
                delete_all(m,val);
                break;
            case DESTROY:
                destroy(m);
                exists = 0;  
                break;
        }
    }
    return "";
}

int main(){

    regex_t *reg;
    Option command;
    char buff[BUFFER];
    char* com;
    struct tms b;
    clock_t usb,usa,syb,sya,reb,rea;
    struct timespec tb,ta;
    #ifdef DYNAMIC
    load_library();
    #endif
    reg = get_regex();
    while(1){

        fgets(buff,BUFFER,stdin);
        times(&b);
        clock_gettime(CLOCK_REALTIME,&tb);
        // reb = clock();
        
        usb = b.tms_utime;
        syb = b.tms_stime;
        command = get_command(buff,reg);
        if(command==END){
            return 0;
        }
        if(command==ERROR){
            fprintf(stderr,"There is no such command\n");
        }
        // sleep(5);
        // for(long int i = 0;i<1000000000;i++){

        // }
        times(&b);
        clock_gettime(CLOCK_REALTIME,&ta);
        
        rea = clock();
        usa = b.tms_utime;
        sya = b.tms_stime;
        printf("%s",exe_command(command,buff));
        clock_gettime(CLOCK_REALTIME,&ta);
        rea = (ta.tv_sec - tb.tv_sec)*CLOCKS_PER_SEC+((ta.tv_nsec - tb.tv_nsec)/1000000000.0)*CLOCKS_PER_SEC;
        // printf("%ld",rea);
        printf("CPU time\n");
        printf("User time:%ld\n",usa-usb);
        printf("System time:%ld\n",sya-syb);
        printf("Real time:%ld\n",rea);
    }
    free(reg); 
    return 0;
}
