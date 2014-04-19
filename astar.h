#ifndef __ASTAR_H
#define __ASTAR_H

typedef struct {
	int x;
	int y;
} point_t;

struct map {
	point_t *field;
	size_t height, width;
};

typedef struct {
	point_t *field;
	size_t length;
} path_t;

extern
path_t *search_path( const point_t start, const point_t target, const struct map *field ); 

#endif /* __ASTAR_H */
