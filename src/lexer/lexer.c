/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2026/02/02 16:26:43 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal/lexer_internal.h"
#include <stdbool.h>

void	lex_step(char **line, t_token *token, t_lex_state *st)
{
	if (!token)
		return ;
	token->token_kind = TK_UNSET;
	skip_blank(line);
	lex_control(line, token);
	if (token->token_kind == TK_EOF && st->paren_depth != 0)
	{
		set_lex_error(token, ERR_UNCLOSED_SUBSHELL);
		return ;
	}
	if (token->token_kind != TK_UNSET)
		return ;
	lex_connect(line, token);
	if (token->token_kind != TK_UNSET)
		return ;
	lex_group(line, token, st);
	if (token->token_kind != TK_UNSET)
		return ;
	lex_redirect(line, token);
	if (token->token_kind != TK_UNSET)
		return ;
	lex_io_number(line, token);
	if (token->token_kind != TK_UNSET)
		return ;
	lex_word(line, token);
}

t_token	*free_tokens(t_token *head, t_token *extra)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->token_kind == TK_WORD)
			free_word_parts(head->u_token.wd.next);
		free(head);
		head = tmp;
	}
	free(extra);
	return (NULL);
}

// Return a token whose token_kind=TK_ERR if and only if quotes don't close.
// Return NULL if and only if malloc fails.
t_token	*tokenize(char *line)
{
	t_token		*head;
	t_token		**tail;
	t_token		*token;
	t_lex_state	st;

	if (!line)
		return (NULL);
	head = NULL;
	tail = &head;
	st.paren_depth = 0;
	while (true)
	{
		token = ft_calloc(1, sizeof(t_token));
		if (!token)
			return (free_tokens(head, NULL));
		lex_step(&line, token, &st);
		if (token->token_kind == TK_UNSET)
			return (free_tokens(head, token));
		if (token->token_kind == TK_ERR)
		{
			free_tokens(head, NULL);
			return (token);
		}
		(*tail) = token;
		tail = &token->next;
		if (token->token_kind == TK_EOF)
			break ;
	}
	return (head);
}
