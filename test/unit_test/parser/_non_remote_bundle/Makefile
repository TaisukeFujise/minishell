NAME_CASES:=parser_cases
NAME_DUMP:=parser_dump_line
NAME_SNAPSHOT:=parser_dump_snapshot

ROOT:=../../..
LIBFT_DIR:=$(ROOT)/libft
LIBFT:=$(LIBFT_DIR)/libft.a

ROOT_SRCS:=parser/parser.c \
	parser/parser_internal/parser_error.c \
	parser/parser_internal/parser_stream.c \
	parser/parser_internal/parser_nodes.c \
	parser/parser_internal/parse_assign.c \
	parser/parser_internal/parse_command.c \
	parser/parser_internal/parse_connective.c \
	parser/parser_internal/parser_heredoc.c \
	lexer/lexer.c \
	lexer/lexer_internal/lex_ops.c \
	lexer/lexer_internal/lex_assign_util.c \
	lexer/lexer_internal/lex_word.c \
	lexer/lexer_internal/lex_word_util.c \
	lexer/lexer_internal/lexer_utils.c

LOCAL_CASE_SRCS:=parser_cases.c cases/parser_cases_suite.c parser_dump.c parser_mock_readline.c
LOCAL_DUMP_SRCS:=parser_dump_line.c parser_dump.c parser_mock_readline.c
LOCAL_SNAPSHOT_SRCS:=parser_dump_snapshot.c cases/parser_snapshot_suite.c parser_dump.c parser_mock_readline.c

ROOT_OBJDIR:=$(ROOT)/obj
TEST_OBJDIR:=obj

ROOT_OBJS:=$(addprefix $(ROOT_OBJDIR)/,$(ROOT_SRCS:%.c=%.o))
CASE_OBJS:=$(LOCAL_CASE_SRCS:%.c=$(TEST_OBJDIR)/%.o)
DUMP_OBJS:=$(LOCAL_DUMP_SRCS:%.c=$(TEST_OBJDIR)/%.o)
SNAPSHOT_OBJS:=$(LOCAL_SNAPSHOT_SRCS:%.c=$(TEST_OBJDIR)/%.o)

CC:=cc
CFLAGS:=-Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/src/parser -I$(ROOT)/libft -I.

all: $(NAME_CASES) $(NAME_DUMP) $(NAME_SNAPSHOT)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME_CASES): $(CASE_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(CASE_OBJS) $(ROOT_OBJS) $(LIBFT)

$(NAME_DUMP): $(DUMP_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(DUMP_OBJS) $(ROOT_OBJS) $(LIBFT)

$(NAME_SNAPSHOT): $(SNAPSHOT_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(SNAPSHOT_OBJS) $(ROOT_OBJS) $(LIBFT)

$(ROOT_OBJDIR)/%.o: $(ROOT)/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TEST_OBJDIR)

fclean: clean
	rm -f $(NAME_CASES) $(NAME_DUMP) $(NAME_SNAPSHOT)

re: fclean all

cases: $(NAME_CASES)

dump: $(NAME_DUMP)

snapshot: $(NAME_SNAPSHOT)

.PHONY: all clean fclean re cases dump snapshot
