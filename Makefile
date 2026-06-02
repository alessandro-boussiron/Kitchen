##
## EPITECH PROJECT, 2026
## Kitchen
## File description:
## Makefile
##

CC      	= 	clang
INCLUDE 	= 	-I./include/
CFLAGS  	= 	-Wall -Wextra $(INCLUDE)

SRCF	    = 	src/
SRC 	    =	$(SRCF)config_parser.c    \
				$(SRCF)config.c           \
				$(SRCF)recipes.c          \
				$(SRCF)commands.c         \
				$(SRCF)cmd_stock.c        \
				$(SRCF)main.c             \

OBJ     = $(SRC:.c=.o)

NAME    = ./kitchen

TEST_OBJ  = src/config_parser.o src/config.o src/recipes.o src/commands.o src/cmd_stock.o
TEST_DIR  = tests
TESTS     = $(TEST_DIR)/test_kitchen \

all:    $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(TEST_OBJ) $(TESTS)
	@for t in $(TESTS); do ./$$t --verbose; done

$(TEST_DIR)/test_kitchen: $(TEST_DIR)/test_kitchen.c $(TEST_OBJ)
	$(CC) $(CFLAGS) $^ -lcriterion -o $@

clean:
	$(RM) $(OBJ) $(TESTS)

fclean: clean
	$(RM) $(NAME)

re:     fclean all
