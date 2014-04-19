#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "load.h"

int has_color = 0;

/* Init ncurses, load map, find path, draw map & path */
int main(int argc, char *argv[])
{
	initscr();
	if((has_color = has_colors()) == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		return 0;
	} else {
		start_color();
		init_printer();
	}
	
	struct map_data *map = load_map("test.map");

	if (!map) {
		fprintf(stderr, "Something went wrong!\n");
		endwin();
		return 1;
	}

	if (!map->data) {
		fprintf(stderr, "Data got corrupted!");
		endwin();
		return 2;
	}

	print_map(map);
#ifdef DEBUG_LOAD_MAP
	printf("%s\n", map->data);
#endif
	refresh();
	getch();
	endwin();

	free(map->data);
	free(map);

	return 0;
}
