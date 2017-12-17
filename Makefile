ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
SRC = src/alloc_core.c src/ft_malloc.c
INC = -Iinclude
OBJECTS = ./src/alloc_core.o ./src/ft_malloc.o
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
