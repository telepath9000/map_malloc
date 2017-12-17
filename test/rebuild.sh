make -C ../ re
cd ../
gcc -Wall -Wextra -Werror -g -I../include -L. ./test/test.c -o test_tmp -lft_malloc_x86_64_Linux
mv test_tmp ./test
cd ./test
mv test_tmp test
