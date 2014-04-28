#ifndef __MAP_H
#define __MAP_H

typedef struct {
	int x, y;
} point_t;

struct map_data {
	char *data;
	int height, width;
};

extern point_t get_start_point( struct map_data *map );
extern point_t get_target_point( struct map_data *map );

#endif
