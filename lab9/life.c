#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void signal_handler(int signo) {}

typedef struct {
	int start;
	int end;
	char** foreground;
	char** background;
} thread_args_t;

void* thread_update_grid(void* args)
{
	thread_args_t* targs = (thread_args_t*)args;
	while (true)
	{
		pause();
		for (int i = targs->start; i < targs->end; i++)
		{
			int row = i / grid_width;
			int col = i % grid_width;
			(*targs->background)[i] = is_alive(row, col, *targs->foreground);
		}
	}
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

// argv[1] - number of threads
int main(int argc, char **argv)
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	struct sigaction sigact;
	sigact.sa_handler = signal_handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGUSR1, &sigact, NULL);

	int threads_number = atoi(argv[1]);
	pthread_t threads[threads_number];
	thread_args_t args[threads_number];
	int cells_per_thread = (grid_width * grid_height / threads_number) +1;
	for (int i = 0; i < threads_number; i++)
	{
		args[i].start = i * cells_per_thread;
		args[i].end = min((i + 1) * cells_per_thread, grid_width * grid_height);
		args[i].foreground = &foreground;
		args[i].background = &background;
		pthread_create(&threads[i], NULL, thread_update_grid, &args[i]);
	}

	init_grid(foreground);

	while (true)
	{
		draw_grid(foreground);
		// Step simulation
		for (int i = 0; i < threads_number; i++)
		{
			pthread_kill(threads[i], SIGUSR1);
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
