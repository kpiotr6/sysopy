#include <stdio.h>
#include <stdlib.h>

int main(){
    char* b = malloc(sizeof(char)*10);
    int* a = b+5;
    *a = 1234;
    for(int i=5;i<10;i++){
        printf("%c\n",*(b+i));
    }
    int* c = (int*)(b+5);
    printf("%d\n",*c);
}
