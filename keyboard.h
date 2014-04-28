#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <ncurses.h>
#include "astar.h"
#include "map.h"

int init_keyboard( int height, int width );
void process_key( int key );
point_t get_cursor();

#endif
