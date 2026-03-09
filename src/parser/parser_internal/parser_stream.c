#include "parser_internal.h"

t_token	*peek(t_parser_state *ps)
{
	t_status	lex_status;

	if (ps->status != ST_OK)
		return (&ps->eof_sentinel);
	if (ps->lookahead.token_kind != TK_UNSET)
		return (&ps->lookahead);
	lex_status = tokenize(&ps->lex, &ps->lookahead);
	if (lex_status == ST_OK)
		return (&ps->lookahead);
	if (lex_status == ST_FAILURE
		&& ps->lookahead.u_token.err == LEX_ERR_MEMORY_ALLOCATION)
		parser_fail(ps, ST_FATAL, NULL);
	else if (lex_status == ST_FAILURE)
		parser_fail(ps, ST_FAILURE, lex_err_msg(ps->lookahead.u_token.err));
	else
		parser_fail(ps, ST_FATAL, NULL);
	return (&ps->eof_sentinel);
}

t_token	consume(t_parser_state *ps)
{
	t_token	tok;

	(void)peek(ps);
	if (ps->status != ST_OK)
		return (ps->eof_sentinel);
	tok = ps->lookahead;
	ps->lookahead.token_kind = TK_UNSET;
	return (tok);
}

int	skip_newline(t_parser_state *ps, int mode)
{
	int	skipped;

	skipped = 0;
	while (ps->status == ST_OK && peek(ps)->token_kind == TK_NEWLINE)
	{
		consume(ps);
		skipped = 1;
		if ((mode & SKIP_AND_COLLECT) && ps->hd_head)
			collect_heredocs(ps);
		if (mode & SKIP_ONE)
			break ;
	}
	if (!skipped && (mode & SKIP_AND_COLLECT) && ps->status == ST_OK
		&& ps->hd_head && peek(ps)->token_kind == TK_EOF)
		collect_heredocs(ps);
	return (skipped);
}
