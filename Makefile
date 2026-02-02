# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fendo <fendo@student.42.jp>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/30 12:42:52 by tafujise          #+#    #+#              #
#    Updated: 2025/12/27 17:46:30 by fendo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=minishell

SRCDIR:=src
SRCS:=main.c\
	  signal/signal_handle.c\
	  lexer/lexer.c\
	  lexer/lexer_internal/lex_ops.c\
	  lexer/lexer_internal/lex_word.c\
	  lexer/lexer_internal/lexer_utils.c\
	  execute/assigns.c\
	  execute/bitmap.c\
	  execute/exec_builtin.c\
	  execute/exec_connection.c\
	  execute/exec_disk.c\
	  execute/exec_null.c\
	  execute/exec_simple.c\
	  execute/exec_subshell.c\
	  execute/execute.c\
	  execute/expand.c\
	  execute/init.c\
	  execute/wait.c\
	  hashmap/hashmap_crud.c\
	  hashmap/hashmap_free.c\
	  hashmap/hashmap_utils.c

HEADDIR:=include
HEADERS:=minishell.h\
		 signal_handle.h\
		 lexer.h\
		 execute.h\
		 parser.h\
		 hashmap.h
HEADERS:=$(addprefix $(HEADDIR)/, $(HEADERS))

LIBFT_DIR:=libft
LIBFT:=$(LIBFT_DIR)/libft.a


OBJDIR:=obj
OBJS:=$(addprefix $(OBJDIR)/,$(SRCS:%.c=%.o))

CC:=cc

CCFLAGS:=-Wall -Wextra -Werror -I$(HEADDIR) -I$(LIBFT_DIR)

DEBUG_FLAGS:=-g -O0

all:$(NAME)

$(LIBFT): FORCE
	make -C $(LIBFT_DIR)
FORCE:

$(NAME): $(OBJS) $(LIBFT)
	$(CC) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

debug: CCFLAGS += $(DEBUG_FLAGS)
debug:
	$(MAKE) fclean
	$(MAKE) -C $(LIBFT_DIR) CCFLAGS="$(CCFLAGS)" all
	$(MAKE) CCFLAGS="$(CCFLAGS)" all

.PHONY: all clean fclean re debug
