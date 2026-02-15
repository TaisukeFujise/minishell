# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/15 15:31:59 by tafujise          #+#    #+#              #
#    Updated: 2026/02/15 15:37:17 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=disk_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := main.c
EXECUTE_SRCS := \
	init_ctx.c \
	fd_bitmap.c\
	exec_null.c \
	pipe_utils.c \
	register_pid.c \
	stdio_guard.c \
	apply_redirect.c \
	assigns.c\
	heredoc_tmpfile.c\
	exec_disk.c\
	args_utils.c\
	path_utils.c\

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
