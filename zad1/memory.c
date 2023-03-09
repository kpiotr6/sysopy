#include "memory.h"
#ifndef DYNAMIC
#include<stdlib.h>
#include<stdio.h>
#include<string.h>



Memory* init(int s){
    // printf("a");
    Memory* m = calloc(1,sizeof(Memory));
    m->blocks = calloc(s,sizeof(char*));
    m->max = s;
    // printf("%d",m->max);
    m->current = 0;
    return m;
}

void count(Memory* m,char* name){
        
    size_t name_len = strlen(name)+17;
    FILE *file; 
    char *buffer = calloc(name_len,sizeof(char));
    char *word;
    if(m->current==m->max){
        return;
    }

    buffer[0]='\0';
    snprintf(buffer,name_len,"wc %s > /tmp/words",name);
    // printf(buffer);
    system(buffer);
    buffer=realloc(buffer,BUFFER*sizeof(char));
    file = fopen("/tmp/words","r");
    if(!file){
        return;
    }
    fgets(buffer,BUFFER,file);
    fclose(file);
    remove("/tmp/words");
    m->blocks[m->current] = buffer;
    m->current++;


}

char* show(Memory *m, int i){
        // printf("c");
    if(m->current<i && m->current<0){
        return "";
    }
    return m->blocks[i];

}

void delete_all(Memory *m,int j){
        // printf("d");
    int to_move;
    if(m->current<j && m->current<0){
        return;
    }
    to_move = m->current - j;
    free(m->blocks[j]);
    for(int i = j+1;i<=m->current;i++){
        m->blocks[i-1] = m->blocks[i]; 
    }
    m->current--;

}

void destroy(Memory *m){
        // printf("e");
    for(int i = m->current-1;i>=0;i--){
        delete_all(m,i);
    }
    free(m->blocks);
    free(m);

}

#endif