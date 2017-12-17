make -C ../
echo "#include "'"../include/ft_malloc.h"'"
#include <stdio.h>

int main(void)
{
	char *ptr;

	ptr = ft_malloc(sizeof(char) * 5);
	ptr[0] = 'a';
	ptr[1] = 'b';
	ptr[2] = 'c';
	ptr[3] = 'd';
	ptr[4] = 0;
	printf("'"sizeof(char) is: %li\\n"'", sizeof(char));
	for (int i = 0; i < 100000; i++)
		ptr = ft_malloc(sizeof(char) * 300);
	return (0);
}" > test.c
cd ../
gcc -Wall -Wextra -Werror -g -I../include -L. ./test/test.c -o test_tmp -lft_malloc_x86_64_Linux
export LD_LIBRARY_PATH=$PWD
mv test_tmp ./test
cd ./test
mv test_tmp test
