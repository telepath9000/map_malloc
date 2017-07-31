ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
SRC = src/
INC = include
OBJECTS = src/main.o
IFLAGS = -L
LFLAGS = -c -fPIC
CFLAGS = -Wall -Werror -Wextra -g
all: $(NAME)
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) $(INC) $(LFLAGS) $(SRC) -o $@
	$(CC) $(OBJECTS) -shared -o $@
clean:
	$(RM) $(OBJECTS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY:re fclean clean all
