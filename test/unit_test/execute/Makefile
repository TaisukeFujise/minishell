# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/03 18:30:51 by tafujise          #+#    #+#              #
#    Updated: 2026/02/04 09:37:58 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=execute_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := msg.c name.c strbuf.c main.c
EXECUTE_SRCS := \
	init_ctx.c \
	pipe_utils.c \
	procs.c \
	apply_redirect.c \
	heredoc_tmpfile.c\
	args_utils.c \
	path_utils.c \
	exec_builtin.c \
	exec_simple.c \
	exec_disk.c \
	wait_children.c \
	signal_handle.c \

VARIABLES_SRCS := env_table.c assign.c
HASHMAP_SRCS := $(notdir $(wildcard $(ROOT)/src/hashmap/*.c))
BUILTIN_SRCS := $(notdir $(wildcard $(ROOT)/src/builtin/*.c))

EXPAND_SRCS := expand.c $(notdir $(wildcard $(ROOT)/src/expand/expand_internal/*.c))

SRCS := $(TEST_SRCS) $(EXECUTE_SRCS) $(HASHMAP_SRCS) $(BUILTIN_SRCS) $(EXPAND_SRCS) $(VARIABLES_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
vpath %.c $(ROOT)/src/strutil \
	$(ROOT)/src/hashmap \
	$(ROOT)/src/execute \
	$(ROOT)/src/execute/dispatch \
	$(ROOT)/src/variables \
	$(ROOT)/src/execute/process \
	$(ROOT)/src/execute/redirect \
	$(ROOT)/src/execute/utils \
	$(ROOT)/src/builtin \
	$(ROOT)/src/expand \
	$(ROOT)/src/expand/expand_internal \
	$(ROOT)/src/signal \
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
