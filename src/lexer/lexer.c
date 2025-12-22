/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2025/12/22 22:48:20 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "minishell.h"
#include "../lexer_internal/lexer_internal.h"
#include "utils.h"
#include "libft.h"

static void	prepare_token(char **line, t_token *token)
{
	if (!token)
		return ;
	token->next = NULL;
	skip_space(line);
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

// return a token whose token_kind=TK_ERR if and only if quotes don't close.
// return NULL if and only if malloc fails.
t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	**tail;
	t_token	*token;

	if (!line)
		return ;
	head = NULL;
	tail = &head;
	while (true)
	{
		token = ft_calloc(1, sizeof(t_token));
		prepare_token(&line, token);
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
