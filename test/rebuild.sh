make -C ../ re
cd ..
gcc -Wall -Werror -Wextra -g -I../include -L. ./test/test.c -o test_tmp -lmap_malloc_$(uname -m)_$(uname -s)
gcc -Wall -Werror -Wextra -g -I../include -L. ./test/million_test.c -o million_test_tmp -lmap_malloc_$(uname -m)_$(uname -s)
mv test_tmp million_test_tmp ./test
cd ./test
mv test_tmp test
mv million_test_tmp million_test
