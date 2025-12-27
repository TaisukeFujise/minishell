NAME := hashmap_test
ROOT := ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := main.c print_test.c
HASHMAP_SRCS := $(notdir $(wildcard $(ROOT)/src/hashmap/*.c))
EXECUTE_SRCS := init.c

SRCS := $(TEST_SRCS) $(HASHMAP_SRCS) $(EXECUTE_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))

# どこにある foo.c でも探す
vpath %.c $(ROOT)/src/hashmap $(ROOT)/src/execute .

LDLIBS := -lreadline

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) $(LDLIBS) -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

