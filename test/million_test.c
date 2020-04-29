#include "../include/map_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char **p;

	p = (char **)map_malloc(sizeof(char *) * 1000000);
	for (size_t i = 0; i < 1000000; i++)
		p[i] = map_malloc(sizeof(char) * (MED_BYTES + 1));
	for (size_t i = 0; i < 1000000; i++)
//		map_free(p[i]);
//	map_free(p);
	return (0);
}
