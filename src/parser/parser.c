/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:23:04 by fendo             #+#    #+#             */
/*   Updated: 2026/03/03 00:40:21 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal/parser_internal.h"

static void	init_parser(char **cursor, t_parser_state *ps,
									t_ctx *ctx, t_arenas *arenas)
{
	ft_bzero(ps, sizeof(t_parser_state));
	init_lex_state(&ps->lex, *cursor, &arenas->ast);
	ps->lookahead.token_kind = TK_UNSET;
	ps->eof_sentinel.token_kind = TK_EOF;
	ps->status = ST_OK;
	ps->ctx = ctx;
	ps->arenas = arenas;
}

static void	clear_parse_ephemeral(t_parser_state *ps)
{
	ps->hd_tail = NULL;
	ps->hd_head = NULL;
	if (ps->lookahead.token_kind != TK_UNSET)
		free_token(&ps->lookahead);
}

void	close_heredocs(t_node *node)
{
	t_redirect	*redir;

	if (!node)
		return ;
	close_heredocs(node->left);
	close_heredocs(node->right);
	redir = NULL;
	if (node->node_kind == NODE_SIMPLE)
		redir = node->u_node.simple_command.redirects;
	else if (node->node_kind == NODE_SUBSHELL)
		redir = node->u_node.subshell.redirects;
	while (redir)
	{
		if (redir->op == REDIR_DLESS)
			redir->hd.raw_str.str = NULL;
		if (redir->hd.content_fd >= 0)
		{
			close(redir->hd.content_fd);
			redir->hd.content_fd = -1;
		}
		redir = redir->next;
	}
}

/*
	parse.y ref (top-level equivalent):
	- inputunit: simple_list simple_list_terminator (L435-L445)
	- command-level parse loop and sync on syntax error
	grammar_ebnf ref:
	- program / linebreak / complete_commands
	Note:
	- The AST construction rules are different from parse.y.
	- While parse.y prompts for additional input for incomplete grammars,
	  this implementation will generate a syntax error.
*/
t_status	parse(char **cursor, t_node *ast, t_ctx *ctx, t_arenas *arenas)
{
	t_parser_state	ps;

	if (!cursor || !*cursor || !ast || !ctx || !arenas)
		return (ST_FATAL);
	ft_bzero(ast, sizeof(t_node));
	ast->node_kind = NODE_COMPLETE;
	init_parser(cursor, &ps, ctx, arenas);
	skip_newline(&ps, SKIP_PLAIN);
	if (ps.status == ST_OK && peek(&ps)->token_kind != TK_EOF)
		ast->left = parse_andor(&ps);
	if (ps.status == ST_OK)
		skip_newline(&ps, SKIP_AND_COLLECT | SKIP_ONE);
	if (ps.status == ST_OK
		&& ps.lookahead.token_kind != TK_UNSET
		&& peek(&ps)->token_kind != TK_EOF)
		parser_fail(&ps, ST_FAILURE, unexpected_token_msg(&ps, peek(&ps)));
	if (ps.status != ST_OK)
		close_heredocs(ast->left);
	if (ps.status == ST_FAILURE)
		sync_next_line(&ps.lex);
	*cursor = ps.lex.line;
	clear_parse_ephemeral(&ps);
	return (ps.status);
}
