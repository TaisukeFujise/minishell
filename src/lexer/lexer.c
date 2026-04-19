/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:37:55 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal/lexer_internal.h"

void	free_token(t_token *token)
{
	if (!token)
		return ;
	token->token_kind = TK_UNSET;
	token->lexeme_begin = NULL;
	token->lexeme_len = 0;
}

void	init_lex_state(t_lex_state *st, char *line, t_arena *arena)
{
	if (!st)
		return ;
	st->line = line;
	st->paren_depth = 0;
	st->arena = arena;
}

void	sync_next_line(t_lex_state *st)
{
	char	*line;

	if (!st || !st->line)
		return ;
	line = st->line;
	while (*line && *line != '\n')
		line++;
	if (*line == '\n')
		line++;
	st->line = line;
}

/*
	parse.y ref:
	- read_token: main dispatch loop (L3557-L3830)
	lex rule:
	- <INITIAL> BLANK+ -> (skip)
	- cascade: lex_control -> lex_connect -> lex_group
	  -> lex_redirect -> lex_io_number -> lex_word
	Common macros (flex-like):
	- BLANK   [ \t]
	- DIGIT   [0-9]
	- NAME_S  [A-Za-z_]
	- NAME_C  [A-Za-z0-9_]
	- NAME    NAME_S NAME_C*
	- BOUND   [ \t\n|()><]
	- DBOUND  (&&|\|\||>>|<<)
	- PLAIN   [^ \t\n|()><'"$*]
*/
t_status	tokenize(t_lex_state *st, t_token *token)
{
	char	*begin;

	if (!st || !token)
		return (ST_FATAL);
	free_token(token);
	ft_strspn(&st->line, " \t", true);
	begin = st->line;
	if (lex_control(&st->line, token) == TK_EOF && st->paren_depth != 0)
		set_lexer_error(token, LEX_ERR_UNCLOSED_SUBSHELL);
	else if (token->token_kind == TK_UNSET
		&& lex_connect(&st->line, token) == TK_UNSET
		&& lex_group(&st->line, token, st) == TK_UNSET
		&& lex_redirect(&st->line, token) == TK_UNSET
		&& lex_io_number(&st->line, token) == TK_UNSET)
		lex_word(&st->line, token, st->arena);
	token->lexeme_begin = begin;
	token->lexeme_len = (size_t)(st->line - begin);
	if (token->token_kind == TK_UNSET)
		return (ST_FATAL);
	if (token->token_kind == TK_ERR)
		return (ST_FAILURE);
	return (ST_OK);
}
