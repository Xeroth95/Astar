#ifndef __MAP_H
#define __MAP_H

struct map_data {
	char *data;
};

extern void print_map(struct map_data *map);
extern void init_printer();

#endif
