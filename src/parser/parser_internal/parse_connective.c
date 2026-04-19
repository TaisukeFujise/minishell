#include "parser_internal.h"

/*
	parse.y ref:
	- command alternatives (L837-L861)
	grammar_ebnf ref:
	- command
*/
t_node	*parse_command(t_parser_state *ps)
{
	if (ps->status != ST_OK)
		return (NULL);
	if (peek(ps)->token_kind == TK_GROUP
		&& peek(ps)->u_token.op_group == GROUP_LPAREN)
		return (parse_subshell(ps));
	if (peek(ps)->token_kind == TK_WORD
		|| peek(ps)->token_kind == TK_REDIR
		|| peek(ps)->token_kind == TK_IO_NUMBER)
		return (parse_simple(ps));
	parser_fail(ps, ST_FAILURE, unexpected_token_msg(ps, peek(ps)));
	return (NULL);
}

/*
	parse.y ref:
	- pipeline: pipeline '|' newline_list pipeline (L1471-L1473)
	grammar_ebnf ref:
	- pipeline
*/
static t_node	*parse_pipe(t_parser_state *ps)
{
	t_node	*left;

	left = parse_command(ps);
	while (ps->status == ST_OK
		&& peek(ps)->token_kind == TK_CONNECT
		&& peek(ps)->u_token.op_connect == CONNECT_PIPE)
	{
		consume(ps);
		skip_newline(ps, SKIP_AND_COLLECT);
		left = make_binary(ps, CONNECT_PIPE, left, parse_command(ps));
	}
	return (left);
}

/*
	parse.y ref:
	- simple_list1 AND_AND/OR_OR newline_list ... (L1390-L1393)
	grammar_ebnf ref:
	- and_or
*/
t_node	*parse_andor(t_parser_state *ps)
{
	t_node			*left;
	t_op_connect	op;

	left = parse_pipe(ps);
	while (ps->status == ST_OK
		&& peek(ps)->token_kind == TK_CONNECT
		&& (peek(ps)->u_token.op_connect == CONNECT_AND_IF
			|| peek(ps)->u_token.op_connect == CONNECT_OR_IF))
	{
		op = consume(ps).u_token.op_connect;
		skip_newline(ps, SKIP_AND_COLLECT);
		left = make_binary(ps, op, left, parse_pipe(ps));
	}
	return (left);
}
