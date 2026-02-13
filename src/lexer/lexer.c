/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2026/02/09 20:14:16 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal/lexer_internal.h"

t_token_kind	lexer_step(char **line, t_token *token, t_lex_state *st)
{
	token->token_kind = TK_UNSET;
	skip_blank(line);
	if (lex_control(line, token) == TK_EOF && st->paren_depth != 0)
	{
		set_lexer_error(token, LEX_ERR_UNCLOSED_SUBSHELL);
		return (token->token_kind);
	}
	if (token->token_kind != TK_UNSET)
		return (token->token_kind);
	if (lex_connect(line, token) != TK_UNSET)
		return (token->token_kind);
	if (lex_group(line, token, st) != TK_UNSET)
		return (token->token_kind);
	if (lex_redirect(line, token) != TK_UNSET)
		return (token->token_kind);
	if (lex_io_number(line, token) != TK_UNSET)
		return (token->token_kind);
	lex_word(line, token);
	return (token->token_kind);
}

int	free_word_parts(t_word *head)
{
	t_word	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
	return (1);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->token_kind == TK_WORD)
		free_word_parts(token->u_token.wd);
	token->token_kind = TK_UNSET;
}

void	init_lex_state(t_lex_state *st, char *line)
{
	if (!st)
		return ;
	st->line = line;
	st->paren_depth = 0;
}

t_status	tokenize(t_lex_state *st, t_token *token)
{
	if (!st || !token)
		return (ST_FATAL);
	if (lexer_step(&st->line, token, st) == TK_UNSET)
		return (ST_FATAL);
	if (token->token_kind == TK_ERR)
		return (ST_FAILURE);
	return (ST_OK);
}
