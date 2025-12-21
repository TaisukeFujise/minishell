/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2025/12/21 22:28:29 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "minishell.h"
#include "lexer_internal.h"
#include "utils.h"

static t_lexeme	peek_lexeme(char *line)
{
	t_lexeme	lexeme;

	skip_space(&line);
	
}

static t_token	*consume_lexeme(char **line, t_lexeme *lxm)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token || !line)
		return (NULL);
	token->token_kind = lxm->tk;
	if (lxm->tk == TK_ERR || lxm->tk == TK_EOF || lxm->tk == TK_NEWLINE)
		return (token);
	if (lxm->tk == TK_CONNECT)
	{
		
		return (token);
	}
	if (lxm->tk == TK_GROUP)
	{
		return (token);
	}
	if (lxm->tk == TK_CONNECT)
	{
		return (token);
	}
	if (lxm->tk == TK_CONNECT)
	{
		return (token);
	}
	if (lxm->tk == TK_CONNECT)
	{
		return (token);
	}
}

static t_token	*generate_token(char **line)
{
	t_lexeme lexeme;

	if (*line == NULL)
		return (consume_lexeme(NULL, NULL));
	lexeme = peek_lexeme(line);
	if (lexeme.tk == TK_ERR)
		return (consume_lexeme(TK_ERR, NULL));
}

void	free_tokens(t_token *head)
{
	t_token *tmp;

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

	head = NULL;
	tail = &head;
	while (true)
	{
		token = prepare_token(&line);
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
