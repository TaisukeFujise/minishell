NAME_CASES:=lexer_cases

ROOT:=../../../..
LIBFT_DIR:=$(ROOT)/libft
LIBFT:=$(LIBFT_DIR)/libft.a

ROOT_SRCS:=strutil/name.c \
	lexer/lexer.c \
	lexer/lexer_internal/lex_ops.c \
	lexer/lexer_internal/lex_assign_util.c \
	lexer/lexer_internal/lex_word.c \
	lexer/lexer_internal/lex_word_util.c \
	lexer/lexer_internal/lexer_utils.c
LOCAL_CASE_SRCS:=lexer_cases.c cases/lexer_cases_suite.c

ROOT_OBJDIR:=$(ROOT)/obj
TEST_OBJDIR:=obj

ROOT_OBJS:=$(addprefix $(ROOT_OBJDIR)/,$(ROOT_SRCS:%.c=%.o))
CASE_OBJS:=$(LOCAL_CASE_SRCS:%.c=$(TEST_OBJDIR)/%.o)

CC:=cc
CFLAGS:=-Wall -Wextra -Werror -I$(ROOT)/include -I$(ROOT)/src/lexer -I$(ROOT)/libft -I.

all: $(NAME_CASES)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME_CASES): $(CASE_OBJS) $(ROOT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(CASE_OBJS) $(ROOT_OBJS) $(LIBFT)

$(ROOT_OBJDIR)/%.o: $(ROOT)/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TEST_OBJDIR)

fclean: clean
	rm -f $(NAME_CASES)

re: fclean all

cases: $(NAME_CASES)

.PHONY: all clean fclean re cases
