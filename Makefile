NAME = libmap_malloc.so
CC = gcc
SRC = src/alloc_core.c src/map_malloc.c src/map_free.c src/utilities.c \
	  src/map_realloc.c src/util_list.c
INC = -I./include
OBJECTS = src/alloc_core.o src/map_malloc.o src/map_free.o src/utilities.o \
		  src/map_realloc.o src/util_list.o
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
