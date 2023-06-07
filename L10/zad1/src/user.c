
#include"chatlim.h"
#include<string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>
int queue;
int end = 0;
int MES_SIZE = sizeof(Msgbuf)-sizeof(long);
int id;
int server;
char queue_name[COMM_LEN];


char** split_string(char* line){
    char** word_array;
    int i = 0;
    char* last;
    char* word = strtok(line," \t\n");
    word_array = malloc(sizeof(char*));
    while(word){
        i++;
        word_array = realloc(word_array,sizeof(char*)*(i+1));
        *(word_array+i-1) = word; 
        word = strtok(NULL," \t\n");
    }
    *(word_array+i) = NULL;
    return word_array;
}

void add_endl(char* line){
    int i = 0;
    while(line[i]!='\0'){
        i++;
    }
    line[i] = '\n';
    line[i+1] = '\0';

}

void handler(int s){
    Msgbuf msg;
    end = 1;
    msg.mtype = STOP;
    msg.id = id;
    mq_close(queue);
    mq_unlink(queue_name);
}



Msgbuf parse(char* line){
    char* commands[] = {"STOP","LIST","2ALL","2ONE"};
    int comm_nr;
    Msgbuf comd;
    int val;
    char **splited;
    int j = 0;
    char buf[COMM_LEN];
    char **tmp;
    for(int i=0;i<4;i++){
        if(strncmp(line,commands[i],4)==0){
            comm_nr = i+1;
            break;
        }
    }
    switch (comm_nr)
    {
        case STOP:
            comd.id = id;
            comd.mtype = STOP;
            end = 1;
            mq_send(server,(const char *)&comd,sizeof(Msgbuf),STOP);
            mq_close(queue);
            mq_unlink(queue_name);
            break;
        case ALL: 
            comd.id = id;
            comd.mtype = ALL;
            splited = split_string(line);
            tmp = splited + 1;
            comd.mess.text[0] = '\0';
            while(*tmp!=NULL){
                strcat(comd.mess.text,*tmp);
                tmp++;
            }
            // memcpy(comd.mess.text,*(splited+1),sizeof(strlen(*splited))); 
            free(splited);
            break;
        case ONE: 
            comd.id = id;
            comd.mtype = ONE;
            splited = split_string(line);
            comd.dest_id = atoi(*(splited+1));
            tmp = splited + 2;
            comd.mess.text[0] = '\0';
            while(*tmp!=NULL){
                strcat(comd.mess.text,*tmp);
                tmp++;
            }
            free(splited);
            break;
        case LIST:
            comd.mtype = LIST;
            comd.id = id;
            break;
    }
    return comd;
}

void analyze_message(Msgbuf *mes){
    Msgbuf new_mes;
    struct tm tm;
    switch(mes->mtype){
        case STOP:
            new_mes.id = id;
            new_mes.mtype = STOP;
            end = 1;
            mq_send(server,(const char *)&mes,sizeof(Msgbuf),STOP);
            printf("Koniec pracy serwera\n");
            mq_close(queue);
            mq_unlink(queue_name);
        break;
        case LIST:
            printf("%s",mes->mess.text);
        break;
        case ALL:
            tm = mes->time;
            printf("Czas: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("Wiadomosc: %s\n",mes->mess.text);
            printf("Od:%d\n",mes->id);
        break;
        case ONE:
            tm = mes->time;
            printf("Czas: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("Wiadomosc: %s\n",mes->mess.text);
            printf("Od:%d\n",mes->id);
        break;
        case INIT:
            id = mes->mess.num;
        break;
    }
}

int main(int argc,char* argv[]){
    //argv[1] is client path
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    Msgbuf mes;
    char buf[2*COMM_LEN];
    char* HOME = getenv("HOME");
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Msgbuf);
    attr.mq_curmsgs = 0;
    signal(SIGINT,handler);
    sprintf(queue_name,"/%s",argv[1]);
    queue = mq_open(queue_name,O_RDWR|O_CREAT|O_EXCL|O_NONBLOCK,0777,&attr);
    server = mq_open("/servq",O_RDWR);
    if(queue == -1){
        perror("blad");
        exit(1);
    }
    mes.mtype = INIT;
    sprintf(mes.mess.text,"%s",queue_name);
    printf("%d\n",server);
    if(mq_send(server,(const char *)&mes,sizeof(Msgbuf),INIT)==-1){
        perror("blad\n");
    };
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    while(!end)
    {
        if(read(STDIN_FILENO,buf,sizeof(char)*2*COMM_LEN)!=-1){
            strtok(buf,"\n");
            add_endl(buf);
            mes = parse(buf);
            mq_send(server,(const char *)&mes,sizeof(Msgbuf),mes.mtype);
            
        }
        if(mq_receive(queue,(char *)&mes,sizeof(Msgbuf),NULL)!=-1){
            analyze_message(&mes);
        }
        sleep(1);
    }
    return 0;
}