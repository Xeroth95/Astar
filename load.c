#include <stdio.h>
#include <stdlib.h>
#include "load.h"
#include "map.h"

int line_length( char *data );
int line_count( char *data );

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
		free( ret );
		fprintf(stderr, "Could not open file %s.\n", path);
		return NULL;
	}

	long int file_size = ftell(f);

#ifdef DEBUG_LOAD_MAP
	printf("Filesize : %d\n", file_size);
#endif

	fclose(f);
	f = fopen(path, "r");
	if (file_size <= 0) {
		fprintf( stderr, "File %s is not a valid map file.\n", path );
		fclose( f );
		free( ret );
		return NULL;
	}

	char *data = malloc(file_size + 1);
	if (!data) {
		free( ret );
		fprintf( stderr, "Could not allocate map buffer.\n" );
		fclose( f );
		return NULL;
	}
		
	int read = fread(data, 1, file_size ,f);
	if (read != file_size) {
		fprintf( stderr, "Could not read \"%s\"", path );
		free( ret );
		free( data );
		fclose( f );
		return NULL;
	}
	data[file_size] = '\0';

	int height = line_count( data );
	int width = line_length( data );

	/* test wether the map is actually valid */
	int i;
	for ( i = 0; i < height; ++i ) {
		if ( line_length( data + i * (width + 1) ) != width) {
			free( ret );
			free( data );
			fprintf( stderr,\
				 "File %s is not a valid map file.\nIt is not quadratic!\n",\
				 path );
			return NULL;
		}
	}

#ifdef DEBUG_LOAD_MAP
	printf("content:\n%s\n", data);
#endif
	ret->data = data;
	ret->height = height;
	ret->width = width;
	

	if (!ret) {
		free( data );
		fprintf(stderr, "Something is wrong here too!");
	}

	return ret;
}

int line_length( char *data )
{
	unsigned int i = 0;
	while ( *data != '\n' ) {
		i++;
		data++;
	}
	return i;
}

int line_count( char *data )
{
	unsigned int i = 0;
	while ( *data != '\0' ) {
		if ( *data == '\n' )
			i++;
		data++;
	}

	return i;
}
