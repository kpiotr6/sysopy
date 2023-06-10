#include "chatlim.h"
#include <stdlib.h>
#include <string.h>
#include <poll.h>


Users* us_init(){
    Users* u = malloc(sizeof(Users));
    u->last_id = 0;
    u->num = 0;
    return u;
}

int us_add(Users *u,int fd){
    if(u->num == MAX_USR-1){
        return -1;
    }
    u->last_id++;
    u->ids[u->num] = u->last_id;
    u->sockets[u->num] = fd;
    u->polls[u->num].fd = fd;
    u->polls[u->num].events = POLLIN;
    u->num++;
    return u->last_id;
}

int us_fd_of_id(Users *u,int uid){
    for(int i=0;i<u->num;i++){
        if(u->ids[i]==uid){
            return u->sockets[i];
        }
    }
}

int us_set_fd_of_id(Users *u,int uid,int fd){
    for(int i=0;i<u->num;i++){
        if(u->ids[i]==uid){
            u->sockets[i] = fd;
            break;
        }
    }
}


void us_remove(Users *u, int user_id){
    for(int i=0;i<u->num;i++){
        if(user_id == u->ids[i]){
            memmove(u->polls+i,u->polls+i+1,sizeof(struct pollfd)*(u->num-1));
            memmove(u->ids+i,u->ids+i+1,sizeof(int)*(u->num-1));
            memmove(u->sockets+i,u->sockets+i+1,sizeof(int)*(u->num-1));
            break;
        }
    }

    u->num--;
}

void us_remove_all(Users *u){
    for(int i=u->num-1;i>0;i--){
        us_remove(u,u->ids[i]);
    }
}

void us_destroy(Users *u){
    us_remove_all(u);
    free(u);
}