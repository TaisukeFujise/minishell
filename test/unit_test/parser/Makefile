NAME_LINE:=parser_dump_line
NAME_REPL:=parser_dump_repl

ROOT:=../../..
LIBFT_DIR:=$(ROOT)/libft
LIBFT:=$(LIBFT_DIR)/libft.a

ROOT_SRCS:=parser/parser.c \
	parser/parser_internal/parser_error.c \
	parser/parser_internal/parser_stream.c \
	parser/parser_internal/parser_nodes.c \
	parser/parser_internal/parser_assign.c \
	parser/parser_internal/parse_command.c \
	parser/parser_internal/parse_connective.c \
	parser/parser_internal/parser_heredoc.c \
	parser/parser_internal/parser_redirect.c \
	lexer/lexer.c \
	lexer/lexer_internal/lex_ops.c \
	lexer/lexer_internal/lex_assign_util.c \
	lexer/lexer_internal/lex_word.c \
	lexer/lexer_internal/lex_word_util.c \
	lexer/lexer_internal/lexer_utils.c
LOCAL_SRCS:=parser_dump.c

LINE_SRCS:=$(LOCAL_SRCS) parser_dump_line.c
REPL_SRCS:=$(LOCAL_SRCS) parser_dump_repl.c

ROOT_OBJDIR:=$(ROOT)/obj
TEST_OBJDIR:=obj

ROOT_OBJS:=$(addprefix $(ROOT_OBJDIR)/,$(ROOT_SRCS:%.c=%.o))
LINE_OBJS:=$(LINE_SRCS:%.c=$(TEST_OBJDIR)/%.o)
REPL_OBJS:=$(REPL_SRCS:%.c=$(TEST_OBJDIR)/%.o)

CC:=cc
CFLAGS:=-Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/src/parser -I$(LIBFT_DIR)

all: $(NAME_LINE) $(NAME_REPL)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME_LINE): $(LINE_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(LINE_OBJS) $(ROOT_OBJS) $(LIBFT) -lreadline

$(NAME_REPL): $(REPL_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(REPL_OBJS) $(ROOT_OBJS) $(LIBFT) -lreadline

$(ROOT_OBJDIR)/%.o: $(ROOT)/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TEST_OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME_LINE) $(NAME_REPL)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

line: $(NAME_LINE)

repl: $(NAME_REPL)

.PHONY: all clean fclean re line repl
