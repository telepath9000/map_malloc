#!/bin/bash

make -C ../
cp ../libmap_malloc_$(uname -m)_$(uname -s).so .
gcc -Wall -Wextra -Werror -g -I../include -L. test.c -o malloc_test -lmap_malloc_$(uname -m)_$(uname -s)
gcc -Wall -Wextra -Werror -g -I../include -L. million_test.c -o million_test -lmap_malloc_$(uname -m)_$(uname -s)
export LD_LIBRARY_PATH=$PWD
