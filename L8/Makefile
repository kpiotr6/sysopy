CC = gcc
CFLAGS = -Wall

all: life
	
life: grid.c life.c
	$(CC) -g $(CFLAGS) grid.c life.c -o life -lncursesw -lpthread
	
clean:
	rm -f life
