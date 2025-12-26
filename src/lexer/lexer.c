/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2025/12/25 23:01:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal/lexer_internal.h"
#include "utils.h"
#include <stdbool.h>

static t_token	*lex_step(char **line, t_token *token)
{
	if (!token)
		return (NULL);
	token->token_kind = TK_UNSET;
	skip_blank(line);
	lex_control(line, token);
	if (token->token_kind != TK_UNSET)
		return (token);
	lex_connect(line, token);
	if (token->token_kind != TK_UNSET)
		return (token);
	lex_group(line, token);
	if (token->token_kind != TK_UNSET)
		return (token);
	lex_redirect(line, token);
	if (token->token_kind != TK_UNSET)
		return (token);
	lex_io_number(line, token);
	if (token->token_kind != TK_UNSET)
		return (token);
	lex_word(line, token);
	return (token);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
	return ;
}

// Return a token whose token_kind=TK_ERR if and only if quotes don't close.
// Return NULL if and only if malloc fails.
t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	**tail;
	t_token	*token;

	if (!line)
		return (NULL);
	head = NULL;
	tail = &head;
	while (true)
	{
		token = ft_calloc(1, sizeof(t_token));
		lex_step(&line, token);
		if (!token || token->token_kind == TK_ERR)
		{
			free_tokens(head);
			return (token);
		}
		(*tail) = token;
		tail = &token->next;
		if (token->token_kind == TK_EOF)
			break ;
	}
	return (head);
}
