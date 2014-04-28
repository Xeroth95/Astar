#include "keyboard.h"

point_t cursor;

char get_key_pressed()
{
	int test = getch();

	switch( test ) {
	case KEY_UP: {
		cursor.y++;
		return '\0';
	}
	case KEY_DOWN: {
		cursor.y--;
		return '\0';
	}
	case KEY_LEFT: {
		cursor.x--;
		return '\0';
	}
	case KEY_RIGHT: {
		cursor.x++;
		return '\0';
	}
	default:
		return (char) test;
	}
}

point_t get_cursor(struct map_data *map)
{
	if ( cursor.x > map->width) {
		cursor.y += cursor.x / map->width;
		cursor.x %= map->width;
	}
	if ( cursor.y > map->height ) {
		cursor.y %= map->height;
	}
	return cursor;
}

void set_cursor(point_t new_cursor)
{
	cursor = new_cursor;
}
