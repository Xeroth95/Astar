#include <ncurses.h>
#include "map.h"

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

void print_path( path_t path )
{
	attron(COLOR_PAIR(PATH));
	int i = 0;
	for (; i < path->length; ++i ) {
		mvprintw(path->field[i].y, path->field[i].x, "%c", 'p');
	}
	attroff(COLOR_PAIR(PATH));
}

void replace_field( struct map_data *map, point_t r, char c )
{
	map->data[r.x + r.y * (map->width + 1)] = c;
}

extern point_t get_start_point( struct map_data *map )
{
	int offset = 0;
	int x,y;
	x = y = 0;
	
	while( map->data[offset] != '\0' ) {
		if (map->data[offset] == 's')
			break;
		++offset;
	}

	if ( map->data[offset] == '\0' ) {
		x = -1;
		y = -1;
	} else {
		y = offset / (map->width + 1);
		x = offset % (map->width + 1);
	}
	point_t t = { x, y };
	return t;
}

extern point_t get_target_point( struct map_data *map )
{
	int offset = 0;
	int x,y;
	x = y = 0;
	
	while( map->data[offset] != '\0' ) {
		if (map->data[offset] == 't')
			break;
		++offset;
	}

	if ( map->data[offset] == '\0' ) {
		x = -1;
		y = -1;
	} else {
		y = offset / (map->width + 1);
		x = offset % (map->width + 1);
	}
	point_t t = { x, y };
	return t;
}
