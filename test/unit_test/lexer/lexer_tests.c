#include "lexer.h"
#include "libft.h"
#include <stdio.h>

typedef struct s_expect_token
{
	t_token_kind	kind;
	const char		*word;
	int				word_len;
	uint16_t		flag;
	t_op_connect	op_connect;
	t_op_group		op_group;
	t_op_redir		op_redir;
	int				io_num;
	int				err;
} 	t_expect_token;

typedef struct s_test_case
{
	const char			*name;
	const char			*input;
	const t_expect_token	*expected;
	size_t				expected_len;
	int					check;
} 	t_test_case;

#define EXP_WORD(s, flg) (t_expect_token){TK_WORD, (s), -1, (flg), 0, 0, 0, 0, 0}
#define EXP_EOF (t_expect_token){TK_EOF, NULL, 0, 0, 0, 0, 0, 0, 0}
#define EXP_NL (t_expect_token){TK_NEWLINE, NULL, 0, 0, 0, 0, 0, 0, 0}
#define EXP_ERR(code) (t_expect_token){TK_ERR, NULL, 0, 0, 0, 0, 0, 0, (code)}
#define EXP_CONNECT(op) (t_expect_token){TK_CONNECT, NULL, 0, 0, (op), 0, 0, 0, 0}
#define EXP_GROUP(op) (t_expect_token){TK_GROUP, NULL, 0, 0, 0, (op), 0, 0, 0}
#define EXP_REDIR(op) (t_expect_token){TK_REDIR, NULL, 0, 0, 0, 0, (op), 0, 0}
#define EXP_IO(fd) (t_expect_token){TK_IO_NUMBER, NULL, 0, 0, 0, 0, 0, (fd), 0}

#define CASE(name, input, exp) (t_test_case){(name), (input), (exp), \
	(sizeof(exp) / sizeof((exp)[0])), 1}
#define CASE_INFO(name, input) (t_test_case){(name), (input), NULL, 0, 0}

static int	match_word(const t_token *tok, const t_expect_token *exp)
{
	int	len;

	if (!exp->word)
		return (0);
	len = exp->word_len;
	if (len < 0)
		len = (int)ft_strlen(exp->word);
	if (tok->u_token.wd.word.len != len)
		return (0);
	if (ft_strncmp(tok->u_token.wd.word.str, exp->word, len) != 0)
		return (0);
	if (tok->u_token.wd.flag != exp->flag)
		return (0);
	return (1);
}

static const char	*token_kind_name(t_token_kind kind)
{
	if (kind == TK_WORD)
		return ("WORD");
	if (kind == TK_CONNECT)
		return ("CONNECT");
	if (kind == TK_GROUP)
		return ("GROUP");
	if (kind == TK_REDIR)
		return ("REDIR");
	if (kind == TK_IO_NUMBER)
		return ("IO_NUMBER");
	if (kind == TK_NEWLINE)
		return ("NEWLINE");
	if (kind == TK_EOF)
		return ("EOF");
	if (kind == TK_ERR)
		return ("ERR");
	return ("UNSET");
}

static const char	*connect_name(t_op_connect op)
{
	if (op == CONNECT_AND_IF)
		return ("&&");
	if (op == CONNECT_OR_IF)
		return ("||");
	if (op == CONNECT_PIPE)
		return ("|");
	return ("NONE");
}

static const char	*group_name(t_op_group op)
{
	if (op == GROUP_LPAREN)
		return ("(");
	if (op == GROUP_RPAREN)
		return (")");
	return ("UNKNOWN");
}

static const char	*redir_name(t_op_redir op)
{
	if (op == REDIR_DGREAT)
		return (">>");
	if (op == REDIR_DLESS)
		return ("<<");
	if (op == REDIR_GREAT)
		return (">");
	if (op == REDIR_LESS)
		return ("<");
	return ("UNKNOWN");
}

static void	print_word_flags(uint16_t flag)
{
	int	first;

	first = 1;
	if (flag & W_SQ)
	{
		if (!first)
			printf("|");
		printf("SQ");
		first = 0;
	}
	if (flag & W_DQ)
	{
		if (!first)
			printf("|");
		printf("DQ");
		first = 0;
	}
	if (flag & W_DOLL)
	{
		if (!first)
			printf("|");
		printf("DOLL");
		first = 0;
	}
	if (flag & W_WILD)
	{
		if (!first)
			printf("|");
		printf("WILD");
		first = 0;
	}
	if (first)
		printf("NONE");
}

static void	print_token(const t_token *tok)
{
	if (!tok)
	{
		printf("  actual: (null)\n");
		return ;
	}
	printf("  actual: %s\n", token_kind_name(tok->token_kind));
	if (tok->token_kind == TK_WORD)
	{
		printf("    flags: ");
		print_word_flags(tok->u_token.wd.flag);
		printf("\n");
		printf("    word : \"%.*s\"\n",
			tok->u_token.wd.word.len, tok->u_token.wd.word.str);
	}
	else if (tok->token_kind == TK_CONNECT)
		printf("    op   : %s\n", connect_name(tok->u_token.op_connect));
	else if (tok->token_kind == TK_GROUP)
		printf("    op   : %s\n", group_name(tok->u_token.op_group));
	else if (tok->token_kind == TK_REDIR)
		printf("    op   : %s\n", redir_name(tok->u_token.op_redir));
	else if (tok->token_kind == TK_IO_NUMBER)
		printf("    fd   : %d\n", tok->u_token.io_num);
	else if (tok->token_kind == TK_ERR)
		printf("    err  : %d\n", tok->u_token.err);
}

static void	print_expected(const t_expect_token *exp)
{
	int	len;

	if (!exp)
	{
		printf("  expect: (null)\n");
		return ;
	}
	printf("  expect: %s\n", token_kind_name(exp->kind));
	if (exp->kind == TK_WORD)
	{
		printf("    flags: ");
		print_word_flags(exp->flag);
		printf("\n");
		len = exp->word_len;
		if (len < 0 && exp->word)
			len = (int)ft_strlen(exp->word);
		if (exp->word)
			printf("    word : \"%.*s\"\n", len, exp->word);
		else
			printf("    word : (null)\n");
	}
	else if (exp->kind == TK_CONNECT)
		printf("    op   : %s\n", connect_name(exp->op_connect));
	else if (exp->kind == TK_GROUP)
		printf("    op   : %s\n", group_name(exp->op_group));
	else if (exp->kind == TK_REDIR)
		printf("    op   : %s\n", redir_name(exp->op_redir));
	else if (exp->kind == TK_IO_NUMBER)
		printf("    fd   : %d\n", exp->io_num);
	else if (exp->kind == TK_ERR)
		printf("    err  : %d\n", exp->err);
}

static void	dump_tokens(const t_token *head)
{
	const t_token	*curr;
	size_t			index;

	curr = head;
	index = 0;
	while (curr)
	{
		printf("  [%zu] %s\n", index, token_kind_name(curr->token_kind));
		if (curr->token_kind == TK_WORD)
		{
			printf("    flags: ");
			print_word_flags(curr->u_token.wd.flag);
			printf("\n");
			printf("    word : \"%.*s\"\n",
				curr->u_token.wd.word.len,
				curr->u_token.wd.word.str);
		}
		else if (curr->token_kind == TK_CONNECT)
			printf("    op   : %s\n", connect_name(curr->u_token.op_connect));
		else if (curr->token_kind == TK_GROUP)
			printf("    op   : %s\n", group_name(curr->u_token.op_group));
		else if (curr->token_kind == TK_REDIR)
			printf("    op   : %s\n", redir_name(curr->u_token.op_redir));
		else if (curr->token_kind == TK_IO_NUMBER)
			printf("    fd   : %d\n", curr->u_token.io_num);
		else if (curr->token_kind == TK_ERR)
			printf("    err  : %d\n", curr->u_token.err);
		curr = curr->next;
		index++;
	}
}

static int	match_token(const t_token *tok, const t_expect_token *exp)
{
	if (!tok || tok->token_kind != exp->kind)
		return (0);
	if (exp->kind == TK_WORD)
		return (match_word(tok, exp));
	if (exp->kind == TK_CONNECT)
		return (tok->u_token.op_connect == exp->op_connect);
	if (exp->kind == TK_GROUP)
		return (tok->u_token.op_group == exp->op_group);
	if (exp->kind == TK_REDIR)
		return (tok->u_token.op_redir == exp->op_redir);
	if (exp->kind == TK_IO_NUMBER)
		return (tok->u_token.io_num == exp->io_num);
	if (exp->kind == TK_ERR)
		return (tok->u_token.err == exp->err);
	return (1);
}

static int	run_case(const t_test_case *tc)
{
	char	*input;
	t_token	*tokens;
	t_token	*curr;
	size_t	i;
	int		ok;

	input = ft_strdup(tc->input);
	if (!input)
	{
		fprintf(stderr, "[FAIL] %s: malloc failed\n", tc->name);
		return (0);
	}
	tokens = tokenize(input);
	if (!tokens)
	{
		fprintf(stderr, "[FAIL] %s: tokenize returned NULL\n", tc->name);
		free(input);
		return (0);
	}
	printf("[CASE] %s\n", tc->name);
	printf("  input : \"%s\"\n", tc->input);
	dump_tokens(tokens);
	curr = tokens;
	ok = 1;
	i = 0;
	if (tc->check)
	{
		while (i < tc->expected_len)
		{
			if (!curr || !match_token(curr, &tc->expected[i]))
			{
				ok = 0;
				break ;
			}
			curr = curr->next;
			i++;
		}
		if (ok && curr != NULL)
			ok = 0;
		if (!ok)
		{
			fprintf(stderr, "  result: FAIL\n");
			if (i < tc->expected_len)
				print_expected(&tc->expected[i]);
			else
				printf("  expect: (no more tokens)\n");
			print_token(curr);
		}
		else
			printf("  result: PASS\n");
	}
	else
		printf("  result: INFO (not validated)\n");
	free_tokens(tokens);
	free(input);
	if (!tc->check)
		return (-1);
	return (ok);
}

int	main(void)
{
	static const t_expect_token	case_empty[] = {EXP_EOF};
	static const t_expect_token	case_spaces[] = {EXP_EOF};
	static const t_expect_token	case_newline[] = {EXP_NL, EXP_EOF};
	static const t_expect_token	case_word[] = {EXP_WORD("echo", W_NONE), EXP_EOF};
	static const t_expect_token	case_words[] = {
		EXP_WORD("echo", W_NONE), EXP_WORD("foo", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_and[] = {
		EXP_WORD("a", W_NONE), EXP_CONNECT(CONNECT_AND_IF),
		EXP_WORD("b", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_or[] = {
		EXP_WORD("a", W_NONE), EXP_CONNECT(CONNECT_OR_IF),
		EXP_WORD("b", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_pipe[] = {
		EXP_WORD("a", W_NONE), EXP_CONNECT(CONNECT_PIPE),
		EXP_WORD("b", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_group[] = {
		EXP_GROUP(GROUP_LPAREN), EXP_WORD("echo", W_NONE),
		EXP_GROUP(GROUP_RPAREN), EXP_EOF
	};
	static const t_expect_token	case_redir[] = {
		EXP_WORD("echo", W_NONE), EXP_REDIR(REDIR_GREAT),
		EXP_WORD("file", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_append[] = {
		EXP_WORD("echo", W_NONE), EXP_REDIR(REDIR_DGREAT),
		EXP_WORD("file", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_heredoc[] = {
		EXP_WORD("cat", W_NONE), EXP_REDIR(REDIR_DLESS),
		EXP_WORD("EOF", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_io_num[] = {
		EXP_IO(2), EXP_REDIR(REDIR_GREAT),
		EXP_WORD("out", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_io_num_space[] = {
		EXP_IO(2), EXP_REDIR(REDIR_GREAT),
		EXP_WORD("out", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_wild[] = {EXP_WORD("*", W_WILD), EXP_EOF};
	static const t_expect_token	case_doll[] = {EXP_WORD("$HOME", W_DOLL), EXP_EOF};
	static const t_expect_token	case_sq[] = {EXP_WORD("'a b'", W_SQ), EXP_EOF};
	static const t_expect_token	case_dq[] = {
		EXP_WORD("\"a $b\"", (W_DQ | W_DOLL)), EXP_EOF
	};
	static const t_expect_token	case_mix[] = {EXP_WORD("a\"b\"c", W_DQ), EXP_EOF};
	static const t_expect_token	case_newline_mid[] = {
		EXP_WORD("a", W_NONE), EXP_NL, EXP_WORD("b", W_NONE), EXP_EOF
	};
	static const t_expect_token	case_unclosed_sq[] = {EXP_ERR(ERR_UNCLOSED_QUOTE)};
	static const t_expect_token	case_unclosed_dq[] = {EXP_ERR(ERR_UNCLOSED_QUOTE)};

	static const t_test_case	cases[] = {
		CASE("empty", "", case_empty),
		CASE("spaces", " \t", case_spaces),
		CASE("newline", "\n", case_newline),
		CASE("word", "echo", case_word),
		CASE("words", "echo foo", case_words),
		CASE("and", "a&&b", case_and),
		CASE("or", "a || b", case_or),
		CASE("pipe", "a|b", case_pipe),
		CASE("group", "(echo)", case_group),
		CASE("redir", "echo>file", case_redir),
		CASE("append", "echo>>file", case_append),
		CASE("heredoc", "cat<<EOF", case_heredoc),
		CASE("io_number", "2>out", case_io_num),
		CASE("io_number_space", "2> out", case_io_num_space),
		CASE("wild", "*", case_wild),
		CASE("dollar", "$HOME", case_doll),
		CASE("single_quote", "'a b'", case_sq),
		CASE("double_quote", "\"a $b\"", case_dq),
		CASE("mixed_quote", "a\"b\"c", case_mix),
		CASE("newline_mid", "a\nb", case_newline_mid),
		CASE("unclosed_sq", "'abc", case_unclosed_sq),
		CASE("unclosed_dq", "\"abc", case_unclosed_dq),
		CASE_INFO("complex_pipe", "echo \"a|b\" | cat -e"),
		CASE_INFO("complex_group", "((echo hi)&& (foo||bar))"),
		CASE_INFO("complex_redir", "2>>out <<EOF"),
		CASE_INFO("complex_mix", "cmd \"a b\"'c d' $HOME/*.c"),
		CASE_INFO("complex_newlines", "a\nb\nc"),
	};

	int		failed;
	int		skipped;
	size_t	i;
	size_t	case_count;
	int		result;

	failed = 0;
	skipped = 0;
	case_count = sizeof(cases) / sizeof(cases[0]);
	i = 0;
	while (i < case_count)
	{
		result = run_case(&cases[i]);
		if (result == 0)
			failed++;
		else if (result < 0)
			skipped++;
		i++;
	}
	if (failed == 0)
		printf("[OK] %zu cases passed, %d info\n",
			case_count - skipped, skipped);
	else
		printf("[NG] %d/%zu cases failed, %d info\n",
			failed, case_count - skipped, skipped);
	return (failed != 0);
}
