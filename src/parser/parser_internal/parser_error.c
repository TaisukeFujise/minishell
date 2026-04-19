#include "parser_internal.h"

static char	*token_text(t_token *tk, size_t *text_len)
{
	if (tk->token_kind == TK_NEWLINE || tk->token_kind == TK_EOF)
	{
		*text_len = ft_strlen(PARSER_MSG_TOKEN_NEWLINE);
		return (PARSER_MSG_TOKEN_NEWLINE);
	}
	if (!tk->lexeme_begin || tk->lexeme_len == 0)
		return (NULL);
	*text_len = tk->lexeme_len;
	return ((char *)tk->lexeme_begin);
}

char	*unexpected_token_msg(t_parser_state *ps, t_token *tk)
{
	char		*text;
	size_t		text_len;
	size_t		prefix_len;
	char		*msg;

	text = token_text(tk, &text_len);
	if (!text)
		return (PARSER_MSG_SYNTAX);
	prefix_len = ft_strlen(PARSER_MSG_UNEXPECTED_PREFIX);
	if (text_len > SIZE_MAX - prefix_len - 1)
		return (PARSER_MSG_SYNTAX);
	msg = ft_arena_alloc(&ps->arenas->ast, prefix_len + text_len + 2);
	if (!msg)
		return (PARSER_MSG_FATAL);
	ft_memcpy(msg, PARSER_MSG_UNEXPECTED_PREFIX, prefix_len);
	ft_memcpy(msg + prefix_len, text, text_len);
	msg[prefix_len + text_len] = '\'';
	msg[prefix_len + text_len + 1] = '\0';
	return (msg);
}

char	*hd_eof_warn_msg(t_parser_state *ps, char *delim)
{
	size_t	plen;
	size_t	dlen;
	size_t	len;
	char	*msg;

	plen = ft_strlen(PARSER_MSG_HD_EOF_PREFIX);
	dlen = ft_strlen(delim);
	len = plen + dlen + ft_strlen(PARSER_MSG_HD_EOF_SUFFIX);
	msg = ft_arena_alloc(&ps->arenas->ast, len + 1);
	if (!msg)
		return (PARSER_MSG_SYNTAX);
	ft_memcpy(msg, PARSER_MSG_HD_EOF_PREFIX, plen);
	ft_memcpy(msg + plen, delim, dlen);
	ft_strlcpy(msg + plen + dlen, PARSER_MSG_HD_EOF_SUFFIX,
		len - plen - dlen + 1);
	return (msg);
}

char	*lex_err_msg(t_lexer_err err)
{
	if (err == LEX_ERR_UNCLOSED_SINGLE_QUOTE)
		return (PARSER_MSG_UNCLOSED_SQ);
	else if (err == LEX_ERR_UNCLOSED_DOUBLE_QUOTE)
		return (PARSER_MSG_UNCLOSED_DQ);
	else if (err == LEX_ERR_UNCLOSED_SUBSHELL)
		return (PARSER_MSG_UNCLOSED_SUBSHELL);
	else if (err == LEX_ERR_MEMORY_ALLOCATION)
		return (PARSER_MSG_FATAL);
	else
		return (PARSER_MSG_SYNTAX);
}

void	parser_fail(t_parser_state *ps, t_status status, char *msg)
{
	ps->status = status;
	if (status == ST_FATAL)
	{
		ps->ctx->err.exit_code = 1;
		if (!msg)
			msg = PARSER_MSG_FATAL;
	}
	else
	{
		ps->ctx->err.exit_code = 2;
		if (!msg)
			msg = PARSER_MSG_SYNTAX;
	}
	ps->ctx->err.err_msg = msg;
}
