#pragma once
#include <time.h>
#include <poll.h>
#define STOP 1
#define LIST 2
#define ALL 3
#define ONE 4
#define INIT 5
#define MAX_TYPE 6
#define COMM_LEN 1024
#define MAX_USR 8
#define KEY_SERV 0
#define LOG_PATH "./log.txt"

typedef union realmess{
    int num;
    char text[COMM_LEN];
} RealMess;

typedef struct tm Time;

typedef struct msgbuf {
    long mtype;
    RealMess mess;
    Time time;
    int dest_id;
    int id;
} Msgbuf;

typedef struct users{
    int ids[MAX_USR];
    int sockets[MAX_USR];
    struct pollfd polls[MAX_USR];
    int last_id;
    int num;    
} Users;



Users* us_init();
int us_add(Users *u,int fd);
void us_remove(Users *u, int user_id);
void us_remove_all(Users *u);
void us_destroy(Users *u);
int us_fd_of_id(Users *u,int uid);
