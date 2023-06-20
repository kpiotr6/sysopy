#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>

#define PORT 7777

int main() {
     int fd = -1;
     if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Error creating socket\n");
     }
    // setbuf(STDOUT_FILENO,NULL);
     struct sockaddr_in addr;
     addr.sin_family = AF_INET;
     addr.sin_port = htons(PORT);
     addr.sin_addr.s_addr = inet_addr("0.0.0.0");
     addr.sin_zero[0] = '\0';
     if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) ==
-1) {
         printf("Error binding\n");
     }
        printf("s");
     if (listen(fd, 3) == -1) {
         printf("Error listening\n");
     }
        printf("k");
     struct sockaddr_in cl_addr;
     int cl_fd = -1;
     socklen_t addr_size = sizeof(cl_addr);

     if ((cl_fd = accept(fd, (struct sockaddr*)&cl_addr, &addr_size)) ==
-1) {
         printf("Error accepting\n");
     }

     char buf[64];
     if (read(cl_fd, buf, 64) == -1) {
         printf("Error receiving message\n");
     }
     printf("Message from client: \"%s\"\n", buf);
     shutdown(fd, SHUT_RDWR);
     close(fd);

     return 0;
}
