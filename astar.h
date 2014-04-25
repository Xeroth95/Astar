#ifndef __ASTAR_H
#define __ASTAR_H

#include "map.h"

typedef struct {
	point_t *field;
	size_t length;
} path_t;

extern
int init_pathfinder( int (*walkable_function)( const point_t, const struct map_data* ) );

extern
int cleanup_pathfinder();

extern
path_t *search_path( const point_t start, const point_t target, const struct map_data *field ); 

#endif /* __ASTAR_H */

