ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
SRC = src/alloc_core.c src/ft_malloc.c
INC = include
OBJECTS = src/alloc_core.o src/ft_malloc.o
IFLAGS = -L
LFLAGS = -c
CFLAGS = -Wall -Werror -Wextra -g
all: $(NAME)
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) $(INC) $(LFLAGS) $(SRC) -o -fPIC $<
	$(CC) $(OBJECTS) -shared -o -fPIC $@
clean:
	$(RM) $(OBJECTS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY:re fclean clean all
