/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 17:38:31 by fendo             #+#    #+#             */
/*   Updated: 2026/02/13 00:04:13 by fendo            ###   ########.fr       */
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
	if (op == REDIR_DGREATER)
		return (">>");
	if (op == REDIR_DLESS)
		return ("<<");
	if (op == REDIR_GREATER)
		return (">");
	if (op == REDIR_LESS)
		return ("<");
	return ("UNKNOWN");
}

static void	print_word_flags(uint8_t flag)
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
	if (flag & W_ASSIGN)
	{
		if (!first)
			printf("|");
		printf("ASSIGN");
		first = 0;
	}
	if (flag & W_APPEND)
	{
		if (!first)
			printf("|");
		printf("APPEND");
		first = 0;
	}
	if (flag & W_ID)
	{
		if (!first)
			printf("|");
		printf("IDENT");
		first = 0;
	}
	if (first)
		printf("NONE");
}

static void	print_word_parts(const t_word *head)
{
	const t_word	*part;
	size_t			index;

	part = head;
	index = 0;
	while (part)
	{
		printf("  part[%zu]: ", index);
		print_word_flags(part->flag);
		printf(" \"%.*s\"\n", part->len, part->str);
		part = part->next;
		index++;
	}
	if (index == 0)
		printf("  part[0]: (none)\n");
}

static void	print_word_value(const t_word *head)
{
	const t_word	*part;

	printf("\"");
	part = head;
	while (part)
	{
		printf("%.*s", part->len, part->str);
		part = part->next;
	}
	printf("\"");
}

void	dump_tokens(char *line)
{
	t_token			token;
	t_status		status;
	size_t			index;
	t_word			*word;
	t_lex_state		st;
	t_arena			arena;

	ft_bzero(&token, sizeof(token));
	ft_arena_init(&arena, ARENA_DEFAULT_CHUNK_SIZE);
	init_lex_state(&st, line, &arena);
	index = 0;
	while (1)
	{
		free_token(&token);
		status = tokenize(&st, &token);
		if (token.token_kind == TK_WORD)
		{
			word = token.u_token.wd;
			printf("[%zu] WORD: ", index);
			print_word_value(word);
			printf("\n");
			print_word_parts(word);
			if (word && (word->flag & (W_ASSIGN | W_APPEND)))
			{
				printf("  position of \'=\' : \"%zu\"\n",
					word->eq_ptr - word->str);
			}
		}
		else
		{
			printf("[%zu] %s\n", index, token_kind_name(token.token_kind));
			if (token.token_kind == TK_CONNECT)
				printf("  op   : %s\n", connect_name(token.u_token.op_connect));
			else if (token.token_kind == TK_GROUP)
				printf("  op   : %s\n", group_name(token.u_token.op_group));
			else if (token.token_kind == TK_REDIR)
				printf("  op   : %s\n", redir_name(token.u_token.op_redir));
			else if (token.token_kind == TK_IO_NUMBER)
				printf("  fd   : %d\n", token.u_token.io_num);
			else if (token.token_kind == TK_ERR)
				printf("  err  : %d\n", token.u_token.err);
		}
		if (status != ST_OK || token.token_kind == TK_EOF)
			break ;
		index++;
	}
	free_token(&token);
	ft_arena_destroy(&arena);
}
