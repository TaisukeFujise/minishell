NAME:=arena_tests

ROOT:=../../..
LIBFT_DIR:=$(ROOT)/libft
LIBFT:=$(LIBFT_DIR)/libft.a

CC:=cc
CFLAGS:=-Wall -Wextra -Werror -I$(LIBFT_DIR)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): arena_tests.c $(LIBFT)
	$(CC) $(CFLAGS) -o $@ arena_tests.c $(LIBFT)

test: $(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all test clean fclean re
