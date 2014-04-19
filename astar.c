#include "astar.h"
#include <stdlib.h>

/* defines */

// first try with cost 1, because we do not allow to go diagonally
#define COST 1

/* private structs */

struct star_node {
	struct star_node *parent, **children;
	point_t *val;
	unsigned int cost_to_goal, cost_from_start, cost;
};

typedef struct {
	struct star_node *start, *end, *cur;
} node_list;

/* function prototypes */

void add(node_list *list, struct star_node *node );
void add_adjacent_nodes( node_list *list, struct star_node *node, const struct map_data *field );
void contains( node_list *list, struct star_node *node );
struct star_node *next( node_list *list );
void reset( node_list *list );


unsigned int estimate_cost_to_goal( point_t current, point_t target );
unsigned int get_cost_from_start(struct star_node parent);

int (*is_traversable)(point_t, struct map_data*);

int init_pathfinder(int (*walkable_function)(point_t, struct map_data*))
{
	is_traversable = walkable_function;
}

path_t *search_path( const point_t start, const point_t target, const struct map_data *field )
{
	node_list closed_list;
	node_list open_list;
	
	int target_reached = 0;
	add( closed_list, start );
	add_adjacent_nodes( open_list, start );
	
	while( !target_reached ) {
	    struct star_node *cur = next( open_list );
	    unsigned int min = cur->cost;
	    struct star_node *best = cur;
	    while ( (cur = next( open_list )) != NULL ) {
		  if ( min > cur->cost ) {
			best = cur;
			min = best->cost;
		  }
		  add( closed_list, best );
		  if ( best->val = target ) {
			target_reached = 1;
			break;
		  }
		  add_adjacent_nodes( open_list, best );
	    }
	}
}

struct star_node *create_node( struct star_node *parent, point_t val, point_t target )
{
	struct star_node *ret 	= malloc(sizeof(struct star_node));
	if (!ret) return NULL;
	ret->parent 		= parent;
	ret->val 		= val;
	ret->cost_from_start 	= get_cost_from_start( parent );
	ret->cost_to_goal	= estimate_cost_to_goal( val, target );
	ret->cost		= ret->cost_from_start + ret->cost_to_goal;
}

unsigned int estimate_cost_to_goal(point_t current, point_t target)
{
	return (abs(current.x - target.x) + abs(current.y - target.y)) * COST;
}

unsigned int get_cost_from_start(struct star_node *parent)
{
	return parent->cost_from_start + COST;
}

