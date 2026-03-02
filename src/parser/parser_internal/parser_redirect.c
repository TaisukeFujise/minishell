#include "parser_internal.h"

/*
	parse.y ref:
	- gather_here_documents pending queue drain (L3120-L3136)
	grammar_ebnf ref:
	- no direct production (io_here body collection phase)
*/
void	collect_heredocs(t_parser_state *ps)
{
	t_hd_item	*item;

	if (ps->hd_head == NULL)
		return ;
	while (ps->hd_head && ps->status == ST_OK)
	{
		item = ps->hd_head;
		ps->hd_head = item->next;
		if (ps->hd_head == NULL)
			ps->hd_tail = NULL;
		collect_one_heredoc(ps, item->redir);
	}
	ps->hd_count = 0;
}

/*
	parse.y ref:
	- push_heredoc queueing (L3106-L3117)
	grammar_ebnf ref:
	- io_here / here_end (semantic enqueue phase)
*/
void	register_heredoc(t_parser_state *ps, t_redirect *redir)
{
	t_hd_item	*item;

	if (ps->hd_count >= HEREDOC_MAX)
		return (parser_fail(ps, ST_FATAL, PARSER_MSG_HD_MAX));
	item = ft_arena_calloc(&ps->arenas->ast, 1, sizeof(t_hd_item));
	if (!item)
		return (parser_fail(ps, ST_FATAL, NULL));
	item->redir = redir;
	if (ps->hd_head == NULL)
		ps->hd_head = item;
	else
		ps->hd_tail->next = item;
	ps->hd_tail = item;
	ps->hd_count++;
}

/*
	parse.y ref:
	- redirection rules and heredoc enqueue (L562-L641, L816-L828)
	- push_heredoc implementation (L3106-L3117)
	grammar_ebnf ref:
	- io_redirect / io_file / io_here / here_end
*/
void	add_redir(t_parser_state *ps, t_redirect **list)
{
	int			fd;
	t_op_redir	op;
	t_redirect	*redir;

	fd = -1;
	if (peek(ps)->token_kind == TK_IO_NUMBER)
		fd = consume(ps).u_token.io_num;
	op = consume(ps).u_token.op_redir;
	if (fd < 0 && (op == REDIR_LESS || op == REDIR_DLESS))
		fd = 0;
	else if (fd < 0)
		fd = 1;
	if (peek(ps)->token_kind != TK_WORD)
		return (parser_fail(ps, ST_FAILURE,
				unexpected_token_msg(ps, peek(ps))));
	redir = ft_arena_calloc(&ps->arenas->ast, 1, sizeof(t_redirect));
	if (!redir)
		return (parser_fail(ps, ST_FATAL, NULL));
	redir->op = op;
	redir->io_number = fd;
	redir->hd.content_fd = -1;
	redir->target = *consume(ps).u_token.wd;
	if (op == REDIR_DLESS)
		register_heredoc(ps, redir);
	ilist_append(list, redir, offsetof(t_redirect, next));
}
