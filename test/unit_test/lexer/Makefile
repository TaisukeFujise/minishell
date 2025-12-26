NAME_LINE:=dump_tokens_line
NAME_REPL:=dump_tokens_repl
NAME_TEST:=lexer_tests

ROOT:=../../..
LIBFT_DIR:=$(ROOT)/libft
LIBFT:=$(LIBFT_DIR)/libft.a

ROOT_SRCS:=lexer/lexer.c \
	lexer/lexer_internal/lex_ops.c \
	lexer/lexer_internal/lex_word.c \
	lexer/lexer_internal/lex_utils.c
LOCAL_SRCS:=dump_tokens.c

LINE_SRCS:=$(LOCAL_SRCS) dump_tokens_line.c
REPL_SRCS:=$(LOCAL_SRCS) dump_tokens_repl.c
TEST_SRCS:=$(LOCAL_SRCS) lexer_tests.c

ROOT_OBJDIR:=$(ROOT)/obj
TEST_OBJDIR:=obj

ROOT_OBJS:=$(addprefix $(ROOT_OBJDIR)/,$(ROOT_SRCS:%.c=%.o))
LINE_OBJS:=$(LINE_SRCS:%.c=$(TEST_OBJDIR)/%.o)
REPL_OBJS:=$(REPL_SRCS:%.c=$(TEST_OBJDIR)/%.o)
TEST_OBJS:=$(TEST_SRCS:%.c=$(TEST_OBJDIR)/%.o)

CC:=cc
CFLAGS:=-Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/src/lexer -I$(LIBFT_DIR)

all: $(NAME_LINE) $(NAME_REPL) $(NAME_TEST)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME_LINE): $(LINE_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(LINE_OBJS) $(ROOT_OBJS) $(LIBFT) -lreadline

$(NAME_REPL): $(REPL_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(REPL_OBJS) $(ROOT_OBJS) $(LIBFT) -lreadline

$(NAME_TEST): $(TEST_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(ROOT_OBJS) $(LIBFT) -lreadline

$(ROOT_OBJDIR)/%.o: $(ROOT)/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TEST_OBJDIR)

fclean: clean
	rm -f $(NAME_LINE) $(NAME_REPL) $(NAME_TEST)

re: fclean all

tests: $(NAME_TEST)

.PHONY: all clean fclean re tests
