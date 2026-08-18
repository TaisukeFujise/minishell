# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/11 15:58:40 by tafujise          #+#    #+#              #
#    Updated: 2026/02/16 02:53:43 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME:=builtin_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := msg.c main.c\
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
				export_cmd.c\
				pwd_cmd.c\
				unset_cmd.c\
				export_print.c

STRUTIL_SRCS := name.c strbuf.c

EXEC_SRCS := init_ctx.c\
			exec_builtin.c\
			exec_simple.c\
			pipe_utils.c\
			procs.c\
			wait_children.c\
			apply_redirect.c\
					args_utils.c\
			heredoc_tmpfile.c\
			path_utils.c\
			exec_disk.c\
			signal_handle.c\

VARIABLES_SRCS := env_table.c assign.c
HASHMAP_SRCS := $(notdir $(wildcard $(ROOT)/src/hashmap/*.c))
EXPAND_SRCS := expand.c $(notdir $(wildcard $(ROOT)/src/expand/expand_internal/*.c))

SRCS := $(TEST_SRCS) $(BUILTIN_SRCS) $(STRUTIL_SRCS) $(HASHMAP_SRCS) $(EXEC_SRCS) \
	$(EXPAND_SRCS) $(VARIABLES_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
vpath %.c $(ROOT)/src/hashmap \
	$(ROOT)/src/execute \
	$(ROOT)/src/execute/dispatch \
	$(ROOT)/src/variables \
	$(ROOT)/src/execute/process \
	$(ROOT)/src/execute/redirect \
	$(ROOT)/src/execute/utils \
	$(ROOT)/src/builtin \
	$(ROOT)/src/strutil \
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

debug: CCFLAGS += $(DEBUG_FLAGS)
debug:
	$(MAKE) fclean
	$(MAKE) -C $(LIBFT_DIR) CCFLAGS="$(CCFLAGS)" all
	$(MAKE) CCFLAGS="$(CCFLAGS)" all

.PHONY: all clean fclean re debug
