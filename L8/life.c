#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <signal.h>


char *foreground;
char *background;

void handler(){
	return;
}

void* check_living(void* grids){
	int *igr = (int*)grids;
	int row = igr[0];
	int col = igr[1];
	free(grids);
	while (true)
	{
		pause();
		background[row * GRID_WIDTH + col] = is_alive(row,col,foreground);
	}
}

int main()
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	signal(SIGUSR1,handler);

	foreground = create_grid();
	background = create_grid();

	char *tmp;

	int* args;
	pthread_t tid[GRID_WIDTH * GRID_HEIGHT];

	for(int i=0;i<GRID_HEIGHT;i++){
		for(int j=0;j<GRID_WIDTH;j++){
			args = malloc(sizeof(int)*2);
			args[0] = i;
			args[1] = j;
			pthread_create(&tid[i*GRID_WIDTH+j],NULL,check_living,(void *)args);
		}
	}

	init_grid(foreground);


	while (true)
	{
		draw_grid(foreground);
		for(int i=0;i<GRID_WIDTH * GRID_HEIGHT;i++){
			pthread_kill(tid[i], SIGUSR1);
		}
		usleep(500 * 1000);

		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
