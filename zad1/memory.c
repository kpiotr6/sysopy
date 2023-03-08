#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define BUFFER 255

typedef struct M{
    char** blocks;
    int* indexes;
    int max;
    int current;
}Memory;

Memory* create(int s){
    Memory* m = calloc(1,sizeof(Memory));
    m->blocks = calloc(s,sizeof(char));
    m->indexes = calloc(s,sizeof(int));
    m->max = s;
    m->current = 0;
    return m;
}

Memory read(Memory* m,char* name){
    FILE *file = fopen("/tmp/words","r"); 
    char *buffer = calloc(strlen(name)+17,sizeof(char));
    char *word;
    buffer[0]='\0';
    strcat(buffer,"wc ");
    strcat(buffer,name);
    strcat(buffer," > /tmp/words");
    buffer=realloc(buffer,BUFFER*sizeof(char));
    fgets(buffer,BUFFER,file);

    // printf(buffer);
    system(buffer);
    
}
