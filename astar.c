#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "astar.h"

/* defines */

// first try with cost 1, because we do not allow to go diagonally
#define COST 1

/* private structs */

struct star_node {
	struct star_node *parent;
	point_t val;
	unsigned int cost_to_goal, cost_from_start, cost;
};

typedef struct {
	struct star_node *end, *cur;
} node_list;

/* function prototypes */

struct star_node *create_node( struct star_node *parent, point_t current, point_t target );

void add(node_list *list, struct star_node *node );
void add_adjacent_nodes( struct star_node *(*list)[4], struct star_node *node, const struct map_data *field, node_list closed, node_list open, point_t target );
int contains( node_list *list, struct star_node *node );
//struct star_node *next( node_list *list );
void reset( node_list *list );
int point_equal( point_t x, point_t y );
void cleanup( node_list list );

unsigned int estimate_cost_to_goal( point_t current, point_t target );
unsigned int get_cost_from_start(struct star_node *parent);

path_t *create_path_from_list( node_list *list );

int (*is_traversable)(const point_t, const struct map_data*);

int init_pathfinder(int (*walkable_function)(const point_t, const struct map_data*))
{
	is_traversable = walkable_function;
	return 0;
}

path_t *search_path( const point_t start, const point_t target, const struct map_data *field )
{
	node_list closed_list = { 0 };
	node_list open_list = { 0 };
	struct star_node *open_canidates[4];
	open_canidates[0] = NULL;
	open_canidates[1] = NULL;
	open_canidates[2] = NULL;
	open_canidates[3] = NULL;
	
	int target_reached = 0;
	struct star_node *start_node = create_node( NULL, start, target );
	add( &closed_list, start_node );
	add_adjacent_nodes( &open_canidates, start_node, field, closed_list, open_list, target );

	while( !target_reached ) {
		int i = 0;
		struct star_node *cur = open_canidates[0];
		printf("current: (%d, %d)\n\r", cur->val.x, cur->val.y );  
		unsigned int min = cur->cost;
		struct star_node *best = cur;
		while ( i < 4 && (cur = open_canidates[++i]) != NULL ) {
			add( &open_list, cur );
			if ( min > cur->cost ) {
				//free( best );
				best = cur;
				min = best->cost;
			} else {
				//free( cur );
			}
		}
		add( &closed_list, best );
		if ( point_equal( best->val, target ) ) {
			target_reached = 1;
			break;
		}
		add_adjacent_nodes( &open_canidates, best, field, closed_list, open_list, target );
	}

	path_t *path_to_return = create_path_from_list( &closed_list );
	
	//begin cleanup
	
	cleanup( closed_list );
	cleanup( open_list );
	
	return path_to_return;
}

struct star_node *create_node( struct star_node *parent, point_t val, point_t target )
{
	struct star_node *ret 	= malloc(sizeof(struct star_node));
	if (!ret) return NULL;
	ret->parent 		= parent;
	ret->val 		= val;
	if ( parent == NULL)
		ret->cost_from_start = 0;
	else
		ret->cost_from_start 	= get_cost_from_start( parent );
	ret->cost_to_goal	= estimate_cost_to_goal( val, target );
	ret->cost		= ret->cost_from_start + ret->cost_to_goal;
	return ret;
}

unsigned int estimate_cost_to_goal(point_t current, point_t target)
{
	return (abs(current.x - target.x) + abs(current.y - target.y)) * COST;
}

unsigned int get_cost_from_start(struct star_node *parent)
{
	return parent->cost_from_start + COST;
}

void add_adjacent_nodes( struct star_node *(*list)[4], struct star_node *node, const struct map_data *field, node_list closed, node_list open, point_t target )
{
	int i = 0;
	
	point_t cur = node->val;
	point_t top = { cur.x, cur.y+1 };
	point_t down = { cur.x, cur.y-1 };
	point_t left = { cur.x-1, cur.y };
	point_t right = { cur.x+1, cur.y };

	// test wether they are in the closed list already


	struct star_node *test = closed.end;
	while ( test != NULL ) {
		if ( point_equal( test->val, top) ) {
			top.x = -1;
		} else if ( point_equal( test->val, down) ) {
			down.x = -1;
		} else if ( point_equal( test->val, left ) ) {
			left.x = -1;
		} else if ( point_equal( test->val, right ) ) {
			right.x = -1;
		}
		test = test->parent;
	}
	test = open.end;
	while ( test != NULL ) {
		if ( point_equal( test->val, top) ) {
			top.x = -1;
		} else if ( point_equal( test->val, down) ) {
			down.x = -1;
		} else if ( point_equal( test->val, left ) ) {
			left.x = -1;
		} else if ( point_equal( test->val, right ) ) {
			right.x = -1;
		}
		test = test->parent;
	}

	if ( is_traversable( top, field ) ) {
		(*list)[i++] = create_node( node, top, target );
		printf("Adding node at : (%d, %d)", top.x, top.y);
	}
	if ( is_traversable( down, field ) ) {
		(*list)[i++] = create_node( node, down, target );
		printf("Adding node at : (%d, %d)", down.x, down.y);
	}
	if ( is_traversable( left, field ) ) {
		(*list)[i++] = create_node( node, left, target );
		printf("Adding node at : (%d, %d)", left.x, left.y);
	}
	if ( is_traversable( right, field ) ) {
		(*list)[i++] = create_node( node, right, target );
		printf("Adding node at : (%d, %d)", right.x, right.y);
	}
	while ( i < 5 ) {
		(*list)[i++] = NULL;
	}
}

void add( node_list *list, struct star_node *node )
{
	if ( node == NULL )
		return;
  
#ifdef DEBUG_PATHFINDING
	assert( list->end == node->parent );
#endif
	list->end = node;
}

int point_equal( point_t x, point_t y )
{
	return (x.x == y.x) && (x.y == y.y);
}

path_t *create_path_from_list( node_list *list )
{
	path_t *ret = malloc( sizeof( path_t ) );
	if ( ret == NULL)
		return NULL;
	size_t size = 0;
	struct star_node *cur = list->end;
	while (cur != NULL) {
		size++;
		cur = cur->parent;
	}
	ret->length = size;
	ret->field = malloc( sizeof( point_t ) * size );
	if ( ret->field == NULL ) {
		free( ret );
		fprintf( stderr, "Run out of memory\n!" );
		return NULL;
	}

	cur = list->end;
	//point_t test = { 0, 0 };
	while ( size > 0 ) {
		ret->field[--size] = cur->val;
		cur = cur->parent;
	}

	return ret;
}

int contains( node_list *list, struct star_node *node )
{
	struct star_node *cur = list->end;
	while ( cur != NULL ) {
		if ( point_equal( cur->val, node->val ) )
			return 1;
	}
	
	return 0;
}

void cleanup( node_list list )
{
  
}