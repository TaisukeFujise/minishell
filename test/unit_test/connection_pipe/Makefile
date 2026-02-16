# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/16 02:31:00 by tafujise          #+#    #+#              #
#    Updated: 2026/02/16 21:43:32 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=connection_pipe_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := main.c exec_stubs.c
EXECUTE_SRCS := \
	init_ctx.c \
	execute.c \
	exec_connection.c \
	exec_simple.c \
	exec_disk.c \
	exec_null.c \
	fd_bitmap.c \
	pipe_utils.c \
	register_pid.c \
	wait_children.c \
	stdio_guard.c \
	apply_redirect.c \
	assigns.c \
	heredoc_tmpfile.c \
	args_utils.c \
	path_utils.c \
	envp_utils.c\

HASHMAP_SRCS := $(notdir $(wildcard $(ROOT)/src/hashmap/*.c))

SRCS := $(TEST_SRCS) $(EXECUTE_SRCS) $(HASHMAP_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
vpath %.c $(ROOT)/src/hashmap \
	$(ROOT)/src/execute \
	$(ROOT)/src/execute/dispatch \
	$(ROOT)/src/execute/expansion \
	$(ROOT)/src/execute/process \
	$(ROOT)/src/execute/redirect \
	$(ROOT)/src/execute/utils \
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
