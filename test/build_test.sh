#!/bin/bash

make -C ../
cp ../libmap_malloc.so .
gcc -Wall -Wextra -Werror -g -I../include -L. test.c -o malloc_test -lmap_malloc
gcc -Wall -Wextra -Werror -g -I../include -L. million_test.c -o million_test -lmap_malloc
export LD_LIBRARY_PATH=$PWD
