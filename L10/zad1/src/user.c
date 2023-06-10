
#include "chatlim.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>

int socket_fd;
int end = 0;
// int MES_SIZE = sizeof(Msgbuf) - sizeof(long);
int id;
// char queue_name[COMM_LEN];

char **split_string(char *line)
{
    char **word_array;
    int i = 0;
    char *last;
    char *word = strtok(line, " \t\n");
    word_array = malloc(sizeof(char *));
    while (word)
    {
        i++;
        word_array = realloc(word_array, sizeof(char *) * (i + 1));
        *(word_array + i - 1) = word;
        word = strtok(NULL, " \t\n");
    }
    *(word_array + i) = NULL;
    return word_array;
}

void add_endl(char *line)
{
    int i = 0;
    while (line[i] != '\0')
    {
        i++;
    }
    line[i] = '\n';
    line[i + 1] = '\0';
}

void handler(int s)
{
    Msgbuf msg;
    end = 1;
    msg.mtype = STOP;
    msg.id = id;
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
}

Msgbuf parse(char *line)
{
    char *commands[] = {"STOP", "LIST", "2ALL", "2ONE"};
    int comm_nr;
    Msgbuf comd;
    int val;
    char **splited;
    int j = 0;
    char buf[COMM_LEN];
    char **tmp;
    for (int i = 0; i < 4; i++)
    {
        if (strncmp(line, commands[i], 4) == 0)
        {
            comm_nr = i + 1;
            break;
        }
    }
    switch (comm_nr)
    {
    case STOP:
        comd.id = id;
        comd.mtype = STOP;
        end = 1;
        write(socket_fd, &comd, sizeof(Msgbuf));
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        // mq_send(server, (const char *)&comd, sizeof(Msgbuf), STOP);
        // mq_close(queue);
        // mq_unlink(queue_name);
        break;
    case ALL:
        comd.id = id;
        comd.mtype = ALL;
        splited = split_string(line);
        tmp = splited + 1;
        comd.mess.text[0] = '\0';
        while (*tmp != NULL)
        {
            strcat(comd.mess.text, *tmp);
            tmp++;
        }
        // memcpy(comd.mess.text,*(splited+1),sizeof(strlen(*splited)));
        free(splited);
        break;
    case ONE:
        comd.id = id;
        comd.mtype = ONE;
        splited = split_string(line);
        comd.dest_id = atoi(*(splited + 1));
        tmp = splited + 2;
        comd.mess.text[0] = '\0';
        while (*tmp != NULL)
        {
            strcat(comd.mess.text, *tmp);
            tmp++;
        }
        free(splited);
        break;
    case LIST:
        comd.mtype = LIST;
        comd.id = id;
        break;
    }
    return comd;
}

void analyze_message(Msgbuf *mes)
{
    Msgbuf new_mes;
    struct tm tm;
    switch (mes->mtype)
    {
    case STOP:
        new_mes.id = id;
        new_mes.mtype = STOP;
        end = 1;
        printf("Koniec pracy serwera\n");
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        break;
    case LIST:
        printf("%s", mes->mess.text);
        break;
    case ALL:
        tm = mes->time;
        printf("Czas: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("Wiadomosc: %s\n", mes->mess.text);
        printf("Od:%d\n", mes->id);
        break;
    case ONE:
        tm = mes->time;
        printf("Czas: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("Wiadomosc: %s\n", mes->mess.text);
        printf("Od:%d\n", mes->id);
        break;
    case INIT:
        id = mes->mess.num;
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    Msgbuf mes;
    char buf[2 * COMM_LEN];
    char *HOME = getenv("HOME");
    char *name = argv[1];
    char *type = argv[2];
    char *adress = argv[3];
    int port = atoi(argv[4]);
    signal(SIGINT, handler);
    setbuf(stdout, NULL);
    struct sockaddr_in tcp_addres = {
        .sin_family = AF_INET,
        .sin_port = htons(port)};

    struct sockaddr_un unix_addres = {
        .sun_family = AF_UNIX,
    };
    if (strcmp(type, "local") == 0)
    {
        strcpy((char *)&unix_addres.sun_path, adress);
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (connect(socket_fd, &unix_addres, sizeof(struct sockaddr_un)) == -1)
        {
            perror("Unable to connect to local websocket");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        inet_pton(AF_INET, adress, &tcp_addres.sin_addr);
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(socket_fd, &tcp_addres, sizeof(struct sockaddr_in)) == -1)
        {
            perror("Unable to connect to tcp websocket");
            exit(EXIT_FAILURE);
        }
    }

    mes.mtype = INIT;
    if (read(socket_fd, &mes, sizeof(Msgbuf)) == -1)
    {
        perror("Unable to recieve init message");
    }
    id = mes.mess.num;
    printf("Made connection with server, id: %d\n", id);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    while (!end)
    {
        if (read(STDIN_FILENO, buf, sizeof(char) * 2 * COMM_LEN) != -1)
        {
            strtok(buf, "\n");
            add_endl(buf);
            mes = parse(buf);
            if (write(socket_fd, &mes, sizeof(Msgbuf)) == -1)
            {
                perror("Unable to write to websocket");
            }
        }
        if (recv(socket_fd, &mes, sizeof(Msgbuf), MSG_DONTWAIT) != -1)
        {
            analyze_message(&mes);
        }
        sleep(1);
    }
    return 0;
}