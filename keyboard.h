#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <ncurses.h>
#include "astar.h"
#include "map.h"

point_t get_cursor();
void set_cursor();
char get_key_pressed();

#endif
