/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 17:38:31 by fendo             #+#    #+#             */
/*   Updated: 2025/12/24 23:37:40 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>

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

void	dump_tokens(t_token *head)
{
	t_token	*curr;
	size_t	index;

	index = 0;
	curr = head;
	while (curr)
	{
		printf("[%zu] %s\n", index, token_kind_name(curr->token_kind));
		if (curr->token_kind == TK_WORD)
		{
			printf("  flags: ");
			print_word_flags(curr->u_token.wd.flag);
			printf("\n");
			printf("  word : \"%.*s\"\n", curr->u_token.wd.word.len,
				curr->u_token.wd.word.str);
		}
		else if (curr->token_kind == TK_CONNECT)
			printf("  op   : %s\n", connect_name(curr->u_token.op_connect));
		else if (curr->token_kind == TK_GROUP)
			printf("  op   : %s\n", group_name(curr->u_token.op_group));
		else if (curr->token_kind == TK_REDIR)
			printf("  op   : %s\n", redir_name(curr->u_token.op_redir));
		else if (curr->token_kind == TK_IO_NUMBER)
			printf("  fd   : %d\n", curr->u_token.io_num);
		else if (curr->token_kind == TK_ERR)
			printf("  err  : %d\n", curr->u_token.err);
		curr = curr->next;
		index++;
	}
}
