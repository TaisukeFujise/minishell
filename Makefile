# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/30 12:42:52 by tafujise          #+#    #+#              #
#    Updated: 2026/04/19 20:15:30 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=minishell

SRCDIR:=src
SRCS:=main.c\
	  signal/signal_handle.c\
	  lexer/lexer.c\
	  lexer/lexer_internal/lex_ops.c\
	  lexer/lexer_internal/lex_assign_util.c\
	  lexer/lexer_internal/lex_word.c\
	  lexer/lexer_internal/lex_word_util.c\
	  lexer/lexer_internal/lexer_utils.c\
	  parser/parser_internal/parse_command.c\
	  parser/parser_internal/parse_connective.c\
	  parser/parser_internal/parser_assign.c\
	  parser/parser_internal/parser_error.c\
	  parser/parser_internal/parser_heredoc.c\
	  parser/parser_internal/parser_nodes.c\
	  parser/parser_internal/parser_redirect.c\
	  parser/parser_internal/parser_stream.c\
	  
	  execute/apply_redirect.c\
	  execute/assigns.c\
	  execute/bitmap.c\
	  execute/collect_child.c\
	  execute/create_filename.c\
	  execute/exec_builtin.c\
	  execute/exec_connection.c\
	  execute/exec_disk.c\
	  execute/exec_null.c\
	  execute/exec_simple.c\
	  execute/exec_subshell.c\
	  execute/execute.c\
	  execute/init_ctx.c\
	  execute/dispatch/exec_builtin.c\
	  execute/dispatch/exec_connection.c\
	  execute/dispatch/exec_disk.c\
	  execute/dispatch/exec_null.c\
	  execute/dispatch/exec_simple.c\
	  execute/dispatch/exec_subshell.c\
	  execute/expansion/assigns.c\
	  execute/expansion/expand.c\
	  execute/process/fd_bitmap.c\
	  execute/process/pipe_utils.c\
	  execute/process/register_pid.c\
	  execute/process/wait_children.c\
	  execute/redirect/apply_redirect.c\
	  execute/redirect/heredoc_tmpfile.c\
	  execute/redirect/stdio_guard.c\
	  execute/utils/args_utils.c\
	  execute/utils/path_utils.c\
	  execute/utils/envp_utils.c\
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
