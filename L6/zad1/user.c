#include"chatlim.h"
#include<string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
int queue;
int end = 0;
int MES_SIZE = sizeof(Msgbuf)-sizeof(long);
int id;
int server;


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
    msgsnd(server,&msg,MES_SIZE,MSG_NOERROR);
    msgctl(queue,IPC_RMID,NULL);
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
            msgsnd(server,&comd,MES_SIZE,MSG_NOERROR);
            msgctl(queue,IPC_RMID,NULL);
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
            msgsnd(server,&new_mes,MES_SIZE,MSG_NOERROR);
            printf("Koniec pracy serwera\n");
            msgctl(queue,IPC_RMID,NULL);
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
            id = new_mes.mess.num;
        break;
    }
}

int main(int argc,char* argv[]){
    //argv[1] is client key, argv[2] is server queue
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    Msgbuf mes;
    char buf[2*COMM_LEN];
    char* HOME = getenv("HOME");
    int key = atoi(argv[1]);
    server = atoi(argv[2]); 
    signal(SIGINT,handler);
    queue = msgget(ftok(HOME,key),IPC_CREAT|IPC_EXCL|0777);
    if(queue == -1){
        perror("blad");
        exit(1);
    }
    mes.mtype = INIT;
    mes.mess.num = queue;
    printf("%d\n",server);
    if(msgsnd(server,&mes,MES_SIZE,0)==-1){
        perror("blad\n");
    };
    msgrcv(queue,&mes,MES_SIZE,INIT,0);
    id = mes.mess.num;
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    while(!end)
    {
        if(read(STDIN_FILENO,buf,sizeof(char)*2*COMM_LEN)!=-1){
            strtok(buf,"\n");
            add_endl(buf);
            mes = parse(buf);
            msgsnd(server,&mes,MES_SIZE,MSG_NOERROR);
        }
        if(msgrcv(queue,&mes,MES_SIZE,-MAX_TYPE,IPC_NOWAIT)!=-1){
            analyze_message(&mes);
        }
        sleep(1);
    }
    return 0;
}