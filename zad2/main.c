#include<string.h>
#include<stdio.h>
#include<regex.h>
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
        // printf("lol2");
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
            // printf("%s\n",word);
            last = word;
        }
        word = strtok(NULL," \t\n");
    }
    // printf(last);
    return last;
}

char* exe_command(Option command, char* line){
    int val;
    char* word;
    switch (command)
    {
    case INIT:
        // printf(get_nth_word(line,2));
        val = atoi(get_nth_word(line,2));
        m = init(val);
        break;
    case COUNT:
        word = get_nth_word(line,2);
        // word[strlen(word)-1]='\0';
        // printf("%s",word);
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
        break;
    case END:
        break;
    case ERROR:
        return "ERROR";
        break;
    }
    return "";
}

int main(){
    load_library();
    // count(init(30),"Makefile");
    regex_t *reg;
    Option command;
    char buff[BUFFER];
    reg = get_regex();
    while(1){
        fgets(buff,BUFFER,stdin);
        command = get_command(buff,reg);
        // printf("%d",command);
        if(command==END){
            return 0;
        }
        printf("%s",exe_command(command,buff));
    }
    free(reg); 
    return 0;
}
