ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
SRC = src/alloc_core.c src/ft_malloc.c src/ft_free.c src/utilities.c \
	  src/ft_realloc.c src/show_alloc_mem.c src/util_list.c \
	  src/utilities2.c
INC = -Iinclude
OBJECTS = src/alloc_core.o src/ft_malloc.o src/ft_free.o src/utilities.o \
		  src/ft_realloc.o src/show_alloc_mem.o src/util_list.o \
		  src/utilities2.o
CFLAGS = -g -fPIC -Wall -Werror -Wextra
LDFLAGS = -shared
all: $(NAME)
$(NAME) : $(OBJECTS)
	$(CC) $(INC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJECTS)
clean:
	$(RM) $(OBJECTS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY:re fclean clean all
