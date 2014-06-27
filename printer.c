#include "printer.h"

#define BARRIER 1
#define PATH 2
#define TARGET 3
#define START 4
#define GRASS 5

int init_printer()
{
	init_pair(BARRIER, COLOR_RED, COLOR_BLACK);
	init_pair(PATH, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(TARGET, COLOR_YELLOW, COLOR_BLACK);
	init_pair(START, COLOR_BLUE, COLOR_BLACK);
	init_pair(GRASS, COLOR_GREEN, COLOR_BLACK);

	return 0;
}

void print_map(struct map_data *map)
{
	if (!map || !map->data) {
		fprintf(stderr, "Karte wurde noch nicht initialisiert.\n");
		return;
	}

	char *data = map->data;

	while(*data) {
		switch (*data) {
		case 'x':
			attron(COLOR_PAIR(BARRIER));
			printw("%c", 'x');
			attroff(COLOR_PAIR(BARRIER));
			break;
		case 't':
			attron(COLOR_PAIR(TARGET));
			printw("%c", 't');
			attroff(COLOR_PAIR(TARGET));
			break;
		case 's':
			attron(COLOR_PAIR(START));
			printw("%c", 's');
			attroff(COLOR_PAIR(START));
			break;
		case 'p':
			attron(COLOR_PAIR(PATH));
			printw("%c", *data);
			attroff(COLOR_PAIR(PATH));
			break;
		default:
			attron(COLOR_PAIR(GRASS));
			printw("%c", *data);
			attroff(COLOR_PAIR(GRASS));
			break;
		}
		data++;
	}
}

void print_path( path_t *path )
{
	attron(COLOR_PAIR(PATH));
	size_t i = 0;
	for (; i < path->length; ++i ) {
		mvprintw(path->field[i].y, path->field[i].x, "%c", 'p');
	}
	attroff(COLOR_PAIR(PATH));
	move(path->field[i-1].y, path->field[i-1].x);
}
