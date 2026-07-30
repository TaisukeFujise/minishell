# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/11 15:58:40 by tafujise          #+#    #+#              #
#    Updated: 2026/02/16 01:32:14 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME:=builtin_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := main.c\
			test_helpers.c\
			echo_test.c\
			cd_test.c\
			env_test.c\
			exit_test.c\
			export_test.c\
			pwd_test.c\
			unset_test.c
BUILTIN_SRCS := cd_cmd.c\
				echo_cmd.c\
				env_cmd.c\
				exit_cmd.c\
				pwd_cmd.c\
				unset_cmd.c\

EXEC_SRCS := init_ctx.c\
			exec_builtin.c\
			pipe_utils.c\
			register_pid.c\
			wait_children.c\
			fd_bitmap.c\
			stdio_guard.c\
			apply_redirect.c\
			assigns.c\
			count_args.c\
			heredoc_tmpfile.c\

HASHMAP_SRCS := $(notdir $(wildcard $(ROOT)/src/hashmap/*.c))

SRCS := $(TEST_SRCS) $(BUILTIN_SRCS) $(HASHMAP_SRCS) $(EXEC_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
vpath %.c $(ROOT)/src/hashmap \
	$(ROOT)/src/execute \
	$(ROOT)/src/execute/dispatch \
	$(ROOT)/src/execute/expansion \
	$(ROOT)/src/execute/process \
	$(ROOT)/src/execute/redirect \
	$(ROOT)/src/execute/utils \
	$(ROOT)/src/builtin \
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

debug: CCFLAGS += $(DEBUG_FLAGS)
debug:
	$(MAKE) fclean
	$(MAKE) -C $(LIBFT_DIR) CCFLAGS="$(CCFLAGS)" all
	$(MAKE) CCFLAGS="$(CCFLAGS)" all

.PHONY: all clean fclean re debug
