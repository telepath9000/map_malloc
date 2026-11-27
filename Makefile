NAME = libmap_malloc.so
CC = gcc
SRC = src/alloc_core.c src/map_malloc.c src/map_free.c src/utilities.c \
	  src/map_realloc.c src/util_list.c src/util_address.c src/util_partition.c \
	  src/api_calloc.c src/api_malloc_usable_size.c src/api_secure_free.c \
	  src/api_posix_memalign.c src/api_reallocarray.c
INC = -I./include
OBJECTS = src/alloc_core.o src/map_malloc.o src/map_free.o src/utilities.o \
	  	src/map_realloc.o src/util_list.o src/util_address.o src/util_partition.o \
		src/api_calloc.o src/api_malloc_usable_size.o src/api_secure_free.o \
		src/api_posix_memalign.o src/api_reallocarray.o
CFLAGS = -g -fPIC -Wall -Werror -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Winit-self -Wuninitialized -Wold-style-definition -Wpointer-arith -O3
LDFLAGS = -shared
all: $(NAME)
$(NAME) : $(OBJECTS)
	$(CC) $(INC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJECTS)
clean:
	$(RM) $(OBJECTS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
install: all
	@cp include/map_malloc.h /usr/local/include
	@echo "\033[33minclude/map_malloc.h -> /usr/local/include/map_malloc.h\033[0m"
	@cp $(NAME) /usr/local/lib/
	@echo "\033[32m$(NAME) -> /usr/local/lib/$(NAME)\033[0m"
	@sudo echo "/usr/local/lib" >> /etc/ld.so.conf
	@ldconfig -n /usr/local/lib/
	@ldconfig
	@echo "\033[36mmap_malloc lib installed\033[0m"
uninstall:
	@rm -fv /usr/local/include/map_malloc.h
	@rm -fv /usr/local/lib/$(NAME)
	@ldconfig -n /usr/local/lib/
.PHONY: re fclean clean all
