#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "chatlim.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <poll.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

int end = 0;
int local_socket_fd;
int tcp_socket_fd;
char* path;
Users* users;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void log_file(char* str){
    FILE* file = fopen(LOG_PATH,"a");
    fwrite(str,sizeof(char),strlen(str),file);
    fclose(file);
}

void message_action(Msgbuf *mes,Users *users){
    RealMess new_mess;
    Msgbuf new_mess_buff;
    time_t t = time(NULL);
    struct tm tm;
    int uid;
    char qid;
    int fd;
    char to_log[2*COMM_LEN];
    tm = *localtime(&t);

    sprintf(to_log,"Czas: %d-%02d-%02d %02d:%02d:%02d ",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(to_log,"%s ID: %d Kod operacji: %ld",to_log,mes->id,mes->mtype);

    switch (mes->mtype)
    {
        case INIT:
            new_mess_buff.mtype = INIT;
            new_mess_buff.mess.num = mes->mess.num;
            fd = us_fd_of_id(users,mes->mess.num);
            if(write(fd,&new_mess_buff,sizeof(Msgbuf))==-1){
                perror("Unable to execute init operation");
            }
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
            fd = us_fd_of_id(users,mes->id);
            if(write(fd,&new_mess_buff,sizeof(Msgbuf))==-1){
                perror("Unable to execute list operation");
            }
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
                    if(write(users->sockets[i],&new_mess_buff,sizeof(Msgbuf))==-1){
                        perror("Unable to execute 2ALL operation");
                    }
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
            fd = us_fd_of_id(users,mes->dest_id);
            if(write(fd,&new_mess_buff,sizeof(Msgbuf))==-1){
                perror("Unable to execute list operation");
            }
            break;
        case STOP:
            uid = mes->id;
            fd = us_fd_of_id(users,uid);
            us_remove(users,uid);
            if(shutdown(fd,SHUT_RDWR)==-1){
                perror("Unable to execute shutdown");
            }
            if(close(fd)==-1){
                perror("Unable to execute close");
            }
            break;
        default:
            break;
    }
    sprintf(to_log,"%s\n",to_log);
    log_file(to_log);
}

void stop_users(Users *u){
    Msgbuf buf;
    buf.mtype = STOP;
    for(int i=0;i<u->num;i++){
        if(write(u->sockets[i],&buf,sizeof(Msgbuf))==-1){
            perror("Unable to execute close");
        }
    }
    while(u->num>0){
        buf.id = u->ids[0];
        message_action(&buf,u);
    }
}

int handler(int s){
    Msgbuf msg;
    end = 1;
    msg.mtype = KEY_SERV;
    msg.mess.text[0]='\0';
    pthread_mutex_lock(&mutex);
    stop_users(users);
    if (shutdown(local_socket_fd, SHUT_RDWR) == -1)
    {
        perror("Unable to shutdown local socket");
    }
    if (shutdown(tcp_socket_fd, SHUT_RDWR) == -1)
    {
        perror("Unable to shutdown tcp socket");
    }
    if (close(tcp_socket_fd) == -1)
    {
        perror("Unable to close local socket");
    }
    if (close(local_socket_fd) == -1)
    {
        perror("Unable to shutdown tcp socket");
    }
    if(unlink(path)==-1){
        perror("Unable to unlink file");
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* accept_new_users(void* args){
        Users* users2;
        int socket_fd;
        Msgbuf msg;
        users = *(Users**)args;
        socket_fd = *(int*)(args+sizeof(Users*)+1);
        free(args);
        msg.mtype = INIT;
        while(true){
            int client_fd = accept(socket_fd,NULL,NULL);
            pthread_mutex_lock(&mutex);
            int id = us_add(users,client_fd);
            msg.mess.num = id;
            message_action(&msg,users);
            pthread_mutex_unlock(&mutex);
        }
        
}

int main(int argc,char* argv[]){
    if(argc!=3){
        printf("Wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    path = argv[2];
    users = us_init();
    Msgbuf msg;
    struct pollfd polls[2];
    pthread_t tid_local;
    pthread_t tid_tcp;
    pthread_t tid_ping;

    setbuf(stdout,NULL);

    struct sockaddr_in tcp_addres={
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    tcp_addres.sin_addr.s_addr=htonl(INADDR_ANY);
    printf("%s\n",inet_ntoa(tcp_addres.sin_addr));

    struct sockaddr_un unix_addres = {
        .sun_family = AF_UNIX,
    };
    strcpy((char*)&unix_addres.sun_path,path);


    signal(SIGINT,(void*)handler);

    local_socket_fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(local_socket_fd == -1){
        perror("Failed to create local socket");
        exit(EXIT_FAILURE);
    }

    tcp_socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(tcp_socket_fd == -1){
        perror("Failed to create tcp socket");
        exit(EXIT_FAILURE);
    }

    if(bind(local_socket_fd,(struct sockaddr*)&unix_addres,sizeof(struct sockaddr_un))==-1){
        perror("Unable to bind local socket");
    }
    if(bind(tcp_socket_fd,(struct sockaddr*)&tcp_addres,sizeof(struct sockaddr_in))==-1){
        perror("Unable to bind tcp socket");
    }

    if(listen(local_socket_fd,1024)==-1){
        perror("Failed to listen for local socket");      
    }
    if(listen(tcp_socket_fd,1024)==-1){
        perror("Failed to listen for tcp socket");      
    }
    void* args = malloc(sizeof(Users*)+sizeof(int));
    memcpy(args,&users,sizeof(Users*));
    memcpy(args+sizeof(Users*)+1,&local_socket_fd,sizeof(int));
    pthread_create(&tid_local,NULL,accept_new_users,args);

    args = malloc(sizeof(Users*)+sizeof(int));
    memcpy(args,&users,sizeof(Users*));
    memcpy(args+sizeof(Users*)+1,&tcp_socket_fd,sizeof(int));
    pthread_create(&tid_tcp,NULL,accept_new_users,args);
    while(!end){
        if(poll(users->polls,users->num,1000)>0){
            for(int i=0;i<users->num;i++){
                if(users->polls[i].revents & POLLIN){
                    if(recv(users->sockets[i],&msg,sizeof(Msgbuf),MSG_WAITALL)<0){
                        perror("Error occured during reading from local socket");
                    }
                    else{
                        pthread_mutex_lock(&mutex);
                        printf("recieved %d\n",msg.mtype);
                        message_action(&msg,users);
                        pthread_mutex_unlock(&mutex);
                    }
                    users->polls[i].revents = users->polls[i].revents & (~POLLIN);
                }
            }
        }
    }
    return 0;
}