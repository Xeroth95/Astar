#include "keyboard.h"

int max_height, max_width;
int cur_x, cur_y;

int init_keyboard( int height, int width )
{
	max_height = height;
	max_width = width;
	
	keypad(stdscr, TRUE);

	return 0;
}

void process_key( int key )
{
	switch( key ) {
	case KEY_UP: {
		if ( cur_y > 0 ) --cur_y;
		move( cur_y, cur_x );
		break;
	}
	case KEY_DOWN: {
		if ( cur_y < max_height ) ++cur_y;
		move( cur_y, cur_x );
		break;
	}
	case KEY_LEFT: {
		if ( cur_x > 0 ) --cur_x;
		move( cur_y, cur_x );
		break;
	}
	case KEY_RIGHT: {
		if ( cur_x < max_width ) ++cur_x;
		move( cur_y, cur_x );
		break;
	}
	}
	
	//printw( "This under me should read : \nCursor on : (%d, %d) (%c)\n", cur_x, cur_y, k );
}

point_t get_cursor() 
{
	point_t ret;
	ret.x = cur_x;
	ret.y = cur_y;
	
	return ret;
}
