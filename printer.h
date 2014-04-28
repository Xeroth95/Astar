#ifndef __PRINTER_H
#define __PRINTER_H

#include <ncurses.h>
#include "map.h"
#include "astar.h"

extern void print_map( struct map_data *map );
void print_path( path_t *path );
extern int init_printer();
#endif