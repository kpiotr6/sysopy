#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "chatlim.h"
#include <unistd.h>
int queue;
int end = 0;
int MES_SIZE = sizeof(Msgbuf)-sizeof(long);



void handler(int s){
    Msgbuf msg;
    end = 1;
    msg.mtype = KEY_SERV;
    msg.mess.text[0]='\0';
    // msgsnd(queue,&msg,MES_SIZE,MSG_NOERROR);
}
void log_file(char* str){
    FILE* file = fopen(LOG_PATH,"a+");
    fwrite(str,sizeof(char*),COMM_LEN,file);
    fclose(file);
}

void message_action(Msgbuf *mes,Users *users){
    RealMess new_mess;
    Msgbuf new_mess_buff;
    time_t t = time(NULL);
    struct tm tm;
    int uid;
    int qid;
    char to_log[2*COMM_LEN];
    tm = *localtime(&t);
    sprintf(to_log,"Czas: %d-%02d-%02d %02d:%02d:%02d ",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(to_log,"%s ID: %d Kod operacji: %d\n\0",to_log,mes->id,mes->mtype);
    switch (mes->mtype)
    {
        case INIT:
            // mes->mess.num;
            uid = us_add(users,mes->mess.num);
            qid = mes->mess.num;
            new_mess.num = uid;
            new_mess_buff.mtype = INIT;
            new_mess_buff.mess = new_mess;
            msgsnd(qid,&new_mess_buff,MES_SIZE,MSG_NOERROR);
            break;
        case LIST:
            sprintf(new_mess.text,"Users:\n");
            for(int i=0;i<users->num;i++){
                if(users->ids[i]!=mes->id){
                    sprintf(new_mess.text,"%s %d \n",new_mess.text,users->ids[i]);
                }
                
            }
            new_mess_buff.mtype = LIST;
            new_mess_buff.mess = new_mess;
            new_mess_buff.id = KEY_SERV;
            new_mess_buff.time = tm;
            qid = us_queue_of_id(users,mes->id);
            msgsnd(qid,&new_mess_buff,MES_SIZE,MSG_NOERROR);
            break;
        case ALL:
            sprintf(to_log,"%s Tresc: %s",to_log,mes->mess.text);
            sprintf(new_mess.text,"%s",mes->mess.text);
            new_mess_buff.mtype = ALL;
            new_mess_buff.mess = new_mess;
            new_mess_buff.time = tm;
            new_mess_buff.id = mes->id;
            for(int i=0;i<users->num;i++){
                uid = users->ids[i];
                if(uid!=mes->id){
                    msgsnd(users->queue_ids[i],&new_mess_buff,MES_SIZE,MSG_NOERROR);
                }
            }
            break;
        case ONE:
            sprintf(to_log,"%s Tresc: %s",to_log,mes->mess.text);
            sprintf(new_mess.text,"%s",mes->mess.text);
            new_mess_buff.mtype = ONE;
            new_mess_buff.mess = new_mess;
            new_mess_buff.time = tm;
            new_mess_buff.id = mes->id;
            qid = us_queue_of_id(users,mes->dest_id);
            msgsnd(qid,&new_mess_buff,MES_SIZE,MSG_NOERROR);
            break;
        case STOP:
            uid = mes->id;
            us_remove(users,uid);
            break;
        default:
            break;
    }
    log_file(to_log);
}

void stop_users(Users *u){
    Msgbuf buf;
    buf.mtype = STOP;
    for(int i=0;i<u->num;i++){
        msgsnd(u->queue_ids[i],&buf,MES_SIZE,MSG_NOERROR);
    }
    while(u->num>0){
        msgrcv(queue,&buf,MES_SIZE,STOP,MSG_NOERROR);
        message_action(&buf,u);
    }
}

int main(){
    char* HOME = getenv("HOME");
    Users* users = us_init();
    Msgbuf buf;
    struct timespec spec;
    signal(SIGINT,handler);
    queue = msgget(ftok(HOME,KEY_SERV),IPC_CREAT|IPC_EXCL|0777);
    if(queue == -1){
        perror("Tworzenie");
        exit(1);
    }
    printf("%d\n",queue);
    while(!end){
        if(msgrcv(queue,&buf,MES_SIZE,-MAX_TYPE,IPC_NOWAIT)!=-1){
            message_action(&buf,users);
        }
        sleep(1);
        
    }
    stop_users(users);
    msgctl(queue,IPC_RMID,NULL);
    return 0;
}