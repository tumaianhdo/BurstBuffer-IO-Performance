#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "debug.h"

int parse_args(int argc, char** argv, char* filename, int *dims, int* npdim, uint64_t* spdim, size_t *elem_size)
{
	int i = 0, j = 0, count = 0;
	// *filename = malloc(128 * sizeof(char));
	sprintf(filename, "%s", argv[1]);
	*dims = atoi(argv[2]);
	count = 2;

	if(argc < 2 + (*dims)*2){
		uloga("Wrong number of arguments!\n");
		return -1;
	}

	for(i = count + 1, j = 0; j < *dims; i++, j++){
		*(npdim+j) = atoi(argv[i]);
	}
	count += *dims;

	for(i = count + 1, j = 0; j < *dims; i++, j++){
		*(spdim+j) = strtoull(argv[i], NULL, 10); 
	}
	count += *dims;

	if(argc >= ++count + 1)
		*elem_size = atoi(argv[count]);
	else
		*elem_size = sizeof(double);

	return 0;
}
