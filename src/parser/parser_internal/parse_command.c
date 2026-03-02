#include "parser_internal.h"

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

/*
	parse.y ref (closest in bash grammar):
	- compound_list/list split by newline_list (L1262-L1307)
	grammar_ebnf ref:
	- complete_commands
*/
t_node	*parse_compound_list(t_parser_state *ps)
{
	t_node	*head;
	t_node	*child;

	child = parse_andor(ps);
	if (ps->status != ST_OK || !child)
		return (NULL);
	head = new_node(ps, NODE_COMPLETE);
	if (!head)
		return (NULL);
	head->left = child;
	if (ps->status == ST_OK
		&& skip_newline(ps, SKIP_AND_COLLECT)
		&& (peek(ps)->token_kind == TK_WORD
			|| peek(ps)->token_kind == TK_REDIR
			|| peek(ps)->token_kind == TK_IO_NUMBER
			|| (peek(ps)->token_kind == TK_GROUP
				&& peek(ps)->u_token.op_group == GROUP_LPAREN)))
		head->right = parse_compound_list(ps);
	return (head);
}
