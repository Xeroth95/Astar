#include "astar.h"

/* private structs */

struct star_node {
	struct star_node *parent;
	struct star_node **next;
};

void add_to_path( path_t path, point_t point );
void remove_from_path( path_t path, point_t point );

path_t *search_path( const point_t start, const point_t target, const struct map *field )
{
	
}
