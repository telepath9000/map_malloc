#include "../include/map_malloc.h"
#include <stdio.h>

int main(void)
{
	char **ptr;

	ptr = map_malloc(sizeof(char *) * 1000);
	for (int i = 0; i < 1000; i++)
	{
		ptr[i] = map_malloc(sizeof(char) * 301);
		ptr[i][300] = 0;
		for (int j = 0; j < 300; j++)
			ptr[i][j] = 'a';
	}
	for (int i = 300; i < 400; i++)
		map_free(ptr[i]);
	for (int i = 300; i < 400; i++)
	{
		if (i % 3)
		{
			ptr[i] = map_malloc(sizeof(char) * 10001);
			ptr[i][10000] = 0;
			for (int j = 0; j < 10000; j++)
				ptr[i][j] = 'b';
		}
		else
		{
			ptr[i] = map_malloc(sizeof(char) * 3001);
			ptr[i][3000] = 0;
			for (int j = 0; j < 3000; j++)
				ptr[i][j] = 'c';
		}
	}
	show_alloc_mem();
	for (int i = 0; i < 1000; i++)
		map_free(ptr[i]);
	map_free(ptr);
	show_alloc_mem();
	return (0);
}
