# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/11 15:58:40 by tafujise          #+#    #+#              #
#    Updated: 2026/02/11 20:04:29 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME:=builtin_test
ROOT:= ../../..

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/libft

LIBFT_DIR := $(ROOT)/libft
LIBFT := $(LIBFT_DIR)/libft.a

TEST_SRCS := main.c
BUILTIN_SRCS := cd_cmd.c\
				echo_cmd.c\
				env_cmd.c\
				exit_cmd.c\
				pwd_cmd.c\
				unset_cmd.c\
# 				export_cmd.c\

EXEC_SRCS := init_ctx.c

HASHMAP_SRCS := $(notdir $(wildcard $(ROOT)/src/hashmap/*.c))

SRCS := $(TEST_SRCS) $(BUILTIN_SRCS) $(HASHMAP_SRCS) $(EXEC_SRCS)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
vpath %.c $(ROOT)/src/hashmap $(ROOT)/src/execute $(ROOT)/src/builtin .

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
