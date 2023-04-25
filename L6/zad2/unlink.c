#include <mqueue.h>
int main(int argc,char* argv[]){
    for(int i=1;i<argc;i++){
        mq_unlink(argv[i]);
    }
}