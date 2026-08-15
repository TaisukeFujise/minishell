#include "lexer.h"
#include "libft.h"
#include "lexer_cases_suite.h"
#include <stdio.h>

static char	*flatten_word(t_word *word)
{
	size_t	len;
	char	*buf;
	char	*cur;
	t_word	*head;

	len = 0;
	head = word;
	while (word)
	{
		len += word->len;
		word = word->next;
	}
	buf = ft_calloc(len + 1, sizeof(char));
	if (!buf)
		return (NULL);
	cur = buf;
	word = head;
	while (word)
	{
		ft_memcpy(cur, word->str, word->len);
		cur += word->len;
		word = word->next;
	}
	return (buf);
}

static t_word	*part_at(t_word *word, int index)
{
	int	i;

	i = 0;
	while (word)
	{
		if (i == index)
			return (word);
		word = word->next;
		i++;
	}
	return (NULL);
}

static int	word_equals(t_word *word, const char *expect)
{
	char	*joined;
	int		ok;

	joined = flatten_word(word);
	if (!joined)
		return (0);
	ok = (ft_strcmp(joined, expect) == 0);
	free(joined);
	return (ok);
}

static int	lexeme_equals(t_token *tk, const char *expect)
{
	size_t	len;

	len = ft_strlen(expect);
	if (tk->lexeme_len != len)
		return (0);
	return (ft_strncmp(tk->lexeme_begin, expect, len) == 0);
}

static int	next_token(t_lex_state *st, t_token *tk, t_status *status)
{
	*status = tokenize(st, tk);
	return (1);
}

static int	case_01_basic_sequence(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = "echo a|cat\n";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "echo")
		|| tk.u_token.wd->flag != W_NONE || !lexeme_equals(&tk, "echo"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "a")
		|| tk.u_token.wd->flag != W_NONE || !lexeme_equals(&tk, "a"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_CONNECT
		|| tk.u_token.op_connect != CONNECT_PIPE || !lexeme_equals(&tk, "|"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "cat") || !lexeme_equals(&tk, "cat"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_NEWLINE
		|| !lexeme_equals(&tk, "\n"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_EOF || tk.lexeme_len != 0)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_02_io_number_edges(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = "2>out 2 >out 99999999999999999999>o";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_IO_NUMBER || tk.u_token.io_num != 2)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_REDIR
		|| tk.u_token.op_redir != REDIR_GREATER)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "out"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "2"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_REDIR
		|| tk.u_token.op_redir != REDIR_GREATER)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "out"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "99999999999999999999"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_REDIR
		|| tk.u_token.op_redir != REDIR_GREATER)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "o"))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_EOF)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_03_assign_and_identifier(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = "A=1 B+=2 _ID 1A=2";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"A=1") || !(tk.u_token.wd->flag & W_ASSIGN)
		|| tk.u_token.wd->eq_pos != 1)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"B+=2") || !(tk.u_token.wd->flag & W_APPEND)
		|| tk.u_token.wd->eq_pos != 2)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"_ID") || tk.u_token.wd->flag != W_NONE)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"1A=2") || tk.u_token.wd->flag != W_NONE || tk.u_token.wd->eq_pos)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_EOF)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_04_quote_dollar_wild_parts(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	t_word		*part;
	char		input[] = "\"a$HOME\" '$X' $USER $1 *";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"a$HOME"))
		return (ft_arena_destroy(&arena), 0);
	part = part_at(tk.u_token.wd, 0);
	if (!part || part->flag != W_DQ || ft_strncmp(part->str, "a", part->len))
		return (ft_arena_destroy(&arena), 0);
	part = part_at(tk.u_token.wd, 1);
	if (!part || part->flag != (W_DQ | W_DOLL)
		|| ft_strncmp(part->str, "$HOME", part->len))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"$X"))
		return (ft_arena_destroy(&arena), 0);
	part = part_at(tk.u_token.wd, 0);
	if (!part || part->flag != W_SQ || ft_strncmp(part->str, "$X", part->len))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"$USER") || !(tk.u_token.wd->flag & W_DOLL))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD
		|| !word_equals(tk.u_token.wd, "$1"))
		return (ft_arena_destroy(&arena), 0);
	part = part_at(tk.u_token.wd, 0);
	if (!part || part->flag != W_NONE || ft_strncmp(part->str, "$", part->len))
		return (ft_arena_destroy(&arena), 0);
	part = part_at(tk.u_token.wd, 1);
	if (!part || part->flag != W_NONE || ft_strncmp(part->str, "1", part->len))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"*") || !(tk.u_token.wd->flag & W_WILD))
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_EOF)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_05_empty_quote_parts(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	t_word		*part;
	char		input[] = "\"\" ''";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	part = part_at(tk.u_token.wd, 0);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"") || !part || part->len != 0 || part->flag != W_DQ)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	part = part_at(tk.u_token.wd, 0);
	if (status != ST_OK || tk.token_kind != TK_WORD || !word_equals(tk.u_token.wd,
			"") || !part || part->len != 0 || part->flag != W_SQ)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_EOF)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_06_unclosed_single_quote(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = "'abc";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_FAILURE || tk.token_kind != TK_ERR
		|| tk.u_token.err != LEX_ERR_UNCLOSED_SINGLE_QUOTE)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_07_unclosed_double_quote(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = "\"abc";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_FAILURE || tk.token_kind != TK_ERR
		|| tk.u_token.err != LEX_ERR_UNCLOSED_DOUBLE_QUOTE)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_08_unclosed_subshell(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = "(";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_GROUP
		|| tk.u_token.op_group != GROUP_LPAREN)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_FAILURE || tk.token_kind != TK_ERR
		|| tk.u_token.err != LEX_ERR_UNCLOSED_SUBSHELL)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_09_unmatched_rparen_is_group(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = ")";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_GROUP
		|| tk.u_token.op_group != GROUP_RPAREN)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_EOF)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_10_lexeme_with_whitespace(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	t_status	status;
	char		input[] = " \techo\t>>out\n";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !lexeme_equals(&tk, "echo")
		|| tk.lexeme_begin != input + 2)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_REDIR || !lexeme_equals(&tk, ">>")
		|| tk.lexeme_begin != input + 7)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_WORD || !lexeme_equals(&tk, "out")
		|| tk.lexeme_begin != input + 9)
		return (ft_arena_destroy(&arena), 0);
	next_token(&st, &tk, &status);
	if (status != ST_OK || tk.token_kind != TK_NEWLINE || tk.lexeme_begin != input
		+ 12 || tk.lexeme_len != 1)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

static int	case_11_sync_next_line(void)
{
	t_lex_state	st;
	char		input[] = "echo a\n| b\nlast";

	init_lex_state(&st, input, NULL);
	sync_next_line(&st);
	if (ft_strcmp(st.line, "| b\nlast") != 0)
		return (0);
	sync_next_line(&st);
	if (ft_strcmp(st.line, "last") != 0)
		return (0);
	sync_next_line(&st);
	if (*st.line != '\0')
		return (0);
	return (1);
}

static int	case_12_null_args_are_fatal(void)
{
	t_arena		arena;
	t_lex_state	st;
	t_token		tk;
	char		input[] = "echo";

	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	ft_bzero(&tk, sizeof(tk));
	init_lex_state(&st, input, &arena);
	if (tokenize(NULL, &tk) != ST_FATAL)
		return (ft_arena_destroy(&arena), 0);
	if (tokenize(&st, NULL) != ST_FATAL)
		return (ft_arena_destroy(&arena), 0);
	ft_arena_destroy(&arena);
	return (1);
}

int	lexer_cases_run(void)
{
	int		i;
	int		passed;
	int		total;
	int		(*cases[])(void) = {case_01_basic_sequence, case_02_io_number_edges,
		case_03_assign_and_identifier, case_04_quote_dollar_wild_parts,
		case_05_empty_quote_parts, case_06_unclosed_single_quote,
		case_07_unclosed_double_quote, case_08_unclosed_subshell,
		case_09_unmatched_rparen_is_group, case_10_lexeme_with_whitespace,
		case_11_sync_next_line, case_12_null_args_are_fatal};

	total = sizeof(cases) / sizeof(cases[0]);
	passed = 0;
	i = 0;
	while (i < total)
	{
		if (cases[i]())
			passed++;
		else
			printf("case_%02d: FAILED\n", i + 1);
		i++;
	}
	printf("lexer cases: %d/%d passed\n", passed, total);
	return (passed != total);
}
