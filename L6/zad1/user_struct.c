#include "chatlim.h"
#include <stdlib.h>
#include <string.h>



Users* us_init(){
    Users* u = malloc(sizeof(Users));
    u->last_id = 0;
    u->num = 0;
    return u;
}

int us_add(Users *u,int qid){
    if(u->num == MAX_USR-1){
        return -1;
    }
    u->last_id++;
    u->ids[u->num] = u->last_id;
    u->queue_ids[u->num] = qid;
    u->num++;
    return u->last_id;
}

int us_queue_of_id(Users *u,int uid){
    for(int i=0;i<u->num;i++){
        if(u->ids[i]==uid){
            return u->queue_ids[i];
        }
    }
}

int us_set_queue_of_id(Users *u,int uid,int qid){
    for(int i=0;i<u->num;i++){
        if(u->ids[i]==uid){
            u->queue_ids[i] = qid;
            break;
        }
    }
}


void us_remove(Users *u, int user_id){
    for(int i=0;i<u->num;i++){
        if(user_id == u->ids[i]){
            memmove(u->ids+i,u->ids+i+1,sizeof(int)*(u->num-1));
            memmove(u->queue_ids+i,u->queue_ids+i+1,sizeof(int)*(u->num-1));
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
    free(u);
}