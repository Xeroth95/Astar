#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#ifdef DEBUG
#include <assert.h>
#else
#define assert( x ) x
#endif
#include "load.h"
#include "astar.h"
#include "keyboard.h"
#include "printer.h"
#include "map.h"

int has_color = 0;

int init(struct map_data **map, int argc, char *argv[]);
int cleanup_all();

int is_traversable_test(const point_t test, const struct map_data *field)
{
	if ( test.x < 0 || test.y < 0 || test.x > field->width || test.y > field->height )
		return 0;
	int pos = test.x + test.y * (field->width + 1);
	if ( field->data[pos] == 'x' )
		return 0;
	return 1;
}

/* Init ncurses, load map, find path, draw map & path */
int old_main(int argc, char *argv[])
{
	/*initscr();
	if((has_color = has_colors()) == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		return 0;
	} else {
		assert( start_color() );
		assert( init_printer() );
		assert( init_pathfinder( is_traversable_test ) );
	}
	
	struct map_data *map;
	
	if ( argc > 1 )
		map = load_map( argv[1] );
	else
		map = load_map("test.map");

	if (!map) {
		fprintf(stderr, "Something went wrong!\n");
		endwin();
		return 1;
	}

	if (!map->data) {
		fprintf(stderr, "Data got corrupted!\n");
		endwin();
		return 2;
		}

	struct map_data *map;

	if (init(&map, argc, argv)) {
		exit( 0 );
	}

	print_map(map);
	printw("Laenge: %u\n", map->height);
	printw("Breite: %u\n", map->width);
#ifdef DEBUG_LOAD_MAP
	printf("%s\n", map->data);
#endif

	refresh();
	getch();

	point_t start = get_start_point( map );
	point_t target = get_target_point( map );

	printw("Go from (%d, %d) to (%d, %d)\n", start.x, start.y, target.x, target.y);

	refresh();
	getch();

	path_t *path = search_path( start, target, map );

	unsigned int i;
	for ( i = 0; i < path->length; ++i ) {
		replace_field( map, path->field[i], 'p' );
	}

	print_map( map );

	refresh();
	getch();

	endwin();

	free(map->data);
	free(map);

	cleanup_pathfinder();
  */
	return 0;
}

int main( int argc, char *argv[] )
{
	struct map_data *map;
	if ( init(&map, argc, argv) ) {
		exit( 0 );
	}
	
	int cur;
	
	point_t start = get_start_point( map );
	

	
	while ( (cur = getch()) != 'q' ) {
		clear();
		print_map( map );
		
		process_key( cur );
		switch ( cur ) {
			default: {
				point_t target = get_cursor();
				//mvprintw( 0, 0, "Cursor was at : (%d, %d)", target.x, target.y);
				path_t *path_to_target = search_path( start, target, map );
				print_path( path_to_target );
				refresh();
			}
		}
		
		refresh();
	}
	
	
	cleanup_all();
	return 0;
}

int init(struct map_data **map, int argc, char *argv[])
{
	// process tags
	char *map_path;
	if ( argc > 1 )
		map_path = argv[1];
	else
		map_path = "test.map";

	*map = load_map( map_path );
	if (!*map) {
		fprintf(stderr, "Could not open map: %s\n", map_path);
		return 1;
	}

	//construct map
	if (!(*map)->data) {
		fprintf(stderr, "Data got corrupted!\n");
		return 1;
	}


	// init screen
	initscr();
	if((has_color = has_colors()) == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		return 1;
	} else {
		assert( start_color() );
		assert( init_printer() );
		assert( init_pathfinder( is_traversable_test ) );
		assert( init_keyboard( (*map)->height, (*map)->width-1 ) );
	}

	
	// get keys
	cbreak();
	raw();

	return 0;
}

int cleanup_all()
{
	endwin();
	return 0;
}

