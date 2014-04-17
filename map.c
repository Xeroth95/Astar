#include <ncurses.h>
#include "map.h"

void print_map(struct map_data *map)
{
	init_pair(1, COLOR_RED, COLOR_BLACK);

	if (!map || !map->data) {
		fprintf(stderr, "Karte wurde noch nicht initialisiert.\n");
		return;
	}

	char *data = map->data;

	while(!data) {
		switch (*data) {
		case 'x':
			attron(COLOR_PAIR(1));
			printw("%c\n", *data);
			attroff(COLOR_PAIR(1));
			break;
		default:
			printw("%c\n", *data);
		}
		data++;
	}
}
