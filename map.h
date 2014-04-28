#ifndef __MAP_H
#define __MAP_H

#include "astar.h"

typedef struct {
	int x, y;
} point_t;

struct map_data {
	char *data;
	int height, width;
};

extern void print_map(struct map_data *map);
extern int init_printer();
extern void replace_field( struct map_data *map,  point_t to_replace, char c );
extern point_t get_start_point( struct map_data *map );
extern point_t get_target_point( struct map_data *map );

#endif
