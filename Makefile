##
## EPITECH PROJECT, 2026
## Kitchen
## File description:
## Makefile
##

CC      = clang
INCLUDE = -I./include/
CFLAGS  = -Wall -Wextra $(INCLUDE)

SRCF    = src/
SRC     = $(SRCF)config_parser.c    \
          $(SRCF)config.c           \
          $(SRCF)recipes.c          \
          $(SRCF)commands.c         \
          $(SRCF)main.c             \

OBJ     = $(SRC:.c=.o)

NAME    = ./kitchen

all:    $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re:     fclean all
