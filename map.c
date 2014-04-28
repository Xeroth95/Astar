#include <ncurses.h>
#include "map.h"

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
