#include <stdio.h>
#include <stdlib.h>
#include "load.h"
#include "map.h"

struct map_data *load_map(char *path)
{
	struct map_data *ret = malloc(sizeof(struct map_data));
	if (!ret) {
		fprintf(stderr, "Could not allocate map buffer.\n");
		return NULL;
	}

	//open the file and get the dimensions
	FILE *f = fopen(path, "a");
	if (!f) {
		fprintf(stderr, "Could not open file %s.\n", path);
		return NULL;
	}

	//fseek(f, 0, SEEK_END);
	long int file_size = ftell(f);
#ifdef DEBUG_LOAD_MAP
	printf("Filesize : %d\n", file_size);
#endif

	fclose(f);
	f = fopen(path, "r");
	if (file_size <= 0) {
		fprintf(stderr, "File %s is not a valid map file.\n", path);
		fclose(f);
		return NULL;
	}

	char *data = malloc(file_size + 1);
	if (!data) {
		fprintf(stderr, "Could not allocate map buffer.\n");
		fclose(f);
		return NULL;
	}
		
	fread(data, file_size, 1, f);
	data[file_size] = '\0';

#ifdef DEBUG_LOAD_MAP
	printf("content:\n%s\n", data);
#endif
	ret->data = data;

	if (!ret) {
		fprintf(stderr, "Something is wrong here too!");
	}

	return ret;
}
