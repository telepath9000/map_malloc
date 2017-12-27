make -C ../
echo "#include "'"../include/ft_malloc.h"'"
#include <stdio.h>

int main(void)
{
	char **ptr;

	ptr = ft_malloc(sizeof(char *) * 1000);
	for (int i = 0; i < 1000; i++)
	{
		ptr[i] = ft_malloc(sizeof(char) * 301);
		ptr[i][300] = 0;
		for (int j = 0; j < 300; j++)
			ptr[i][j] = 'a';
	}
	for (int i = 300; i < 400; i++)
		ft_free(ptr[i]);
	for (int i = 300; i < 400; i++)
	{
		if (i % 3)
		{
			ptr[i] = ft_malloc(sizeof(char) * 10001);
			ptr[i][10000] = 0;
			for (int j = 0; j < 10000; j++)
				ptr[i][j] = 'b';
		}
		else
		{
			ptr[i] = ft_malloc(sizeof(char) * 3001);
			ptr[i][3000] = 0;
			for (int j = 0; j < 3000; j++)
				ptr[i][j] = 'c';
		}
	}
	show_alloc_mem();
	for (int i = 0; i < 1000; i++)
		ft_free(ptr[i]);
	ft_free(ptr);
	show_alloc_mem();
	return (0);
}" > test.c
cd ../
gcc -Wall -Wextra -Werror -g -I../include -L. ./test/test.c -o test_tmp -lft_malloc_$(uname -m)_$(uname -s)
export LD_LIBRARY_PATH=$PWD
mv test_tmp ./test
cd ./test
mv test_tmp test
