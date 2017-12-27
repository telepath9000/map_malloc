make -C ../ re
cd ..
gcc -Wall -Werror -Wextra -g -I../include -L. ./test/test.c -o test_tmp -lft_malloc_$(uname -m)_$(uname -s)
mv test_tmp ./test
cd ./test
mv test_tmp test
