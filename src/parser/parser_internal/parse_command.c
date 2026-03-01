#include "parser_internal.h"

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
static void	add_redir(t_parser_state *ps, t_redirect **list)
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

static void	append_simple_word(t_parser_state *ps, t_node *node, t_token tk)
{
	t_assign	*assign;
	t_word_list	*arg;

	if (!node->u_node.simple_command.args
		&& (tk.u_token.wd->flag & (W_ASSIGN | W_APPEND)))
	{
		assign = word_to_assign(ps, tk.u_token.wd);
		if (assign)
			ilist_append(&node->u_node.simple_command.assigns, assign,
				offsetof(t_assign, next));
		return ;
	}
	arg = ft_arena_calloc(&ps->arenas->ast, 1, sizeof(t_word_list));
	if (!arg)
		return (parser_fail(ps, ST_FATAL, NULL));
	arg->wd = tk.u_token.wd;
	ilist_append(&node->u_node.simple_command.args, arg,
		offsetof(t_word_list, next));
}

/*
	parse.y ref:
	- simple_command_element (L804-L813)
	- simple_command accumulation (L831-L835)
	grammar_ebnf ref:
	- simple_command / cmd_prefix / cmd_suffix / cmd_name / cmd_word
*/
t_node	*parse_simple(t_parser_state *ps)
{
	t_node	*node;

	node = new_node(ps, NODE_SIMPLE);
	if (!node)
		return (NULL);
	while (ps->status == ST_OK)
	{
		if (peek(ps)->token_kind == TK_REDIR
			|| peek(ps)->token_kind == TK_IO_NUMBER)
			add_redir(ps, &node->u_node.simple_command.redirects);
		else if (peek(ps)->token_kind == TK_WORD)
			append_simple_word(ps, node, consume(ps));
		else
			break ;
	}
	if (ps->status == ST_OK && !node->u_node.simple_command.assigns
		&& !node->u_node.simple_command.args
		&& !node->u_node.simple_command.redirects)
		parser_fail(ps, ST_FAILURE, unexpected_token_msg(ps, peek(ps)));
	if (ps->status != ST_OK)
		return (NULL);
	return (node);
}

/*
	parse.y ref:
	- subshell: '(' compound_list ')' (L1097-L1103)
	- compound_list newline handling (L1262-L1271)
	- shell_command redirection_list merge (L841-L856)
	grammar_ebnf ref:
	- command / subshell / compound_list
*/
t_node	*parse_subshell(t_parser_state *ps)
{
	t_node	*node;

	consume(ps);
	skip_newline(ps, SKIP_AND_COLLECT);
	node = new_node(ps, NODE_SUBSHELL);
	if (!node)
		return (NULL);
	node->left = parse_compound_list(ps);
	skip_newline(ps, SKIP_AND_COLLECT);
	if (ps->status == ST_OK
		&& (peek(ps)->token_kind != TK_GROUP
			|| peek(ps)->u_token.op_group != GROUP_RPAREN))
		parser_fail(ps, ST_FAILURE, unexpected_token_msg(ps, peek(ps)));
	else if (ps->status == ST_OK)
		consume(ps);
	while (ps->status == ST_OK
		&& (peek(ps)->token_kind == TK_REDIR
			|| peek(ps)->token_kind == TK_IO_NUMBER))
		add_redir(ps, &node->u_node.subshell.redirects);
	if (ps->status != ST_OK)
		return (NULL);
	return (node);
}
