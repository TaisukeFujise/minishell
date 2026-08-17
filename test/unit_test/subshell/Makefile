# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/17 00:00:00 by codex             #+#    #+#              #
#    Updated: 2026/02/17 00:00:00 by codex            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=subshell_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := main.c exec_stubs.c
EXECUTE_SRCS := \
	exec_subshell.c \
	pipe_utils.c \
	register_pid.c \
	wait_children.c

SRCS := $(TEST_SRCS) $(EXECUTE_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
vpath %.c $(ROOT)/src/execute \
	$(ROOT)/src/execute/dispatch \
	$(ROOT)/src/execute/process \
	.

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
