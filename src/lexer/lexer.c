/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:32:33 by fendo             #+#    #+#             */
/*   Updated: 2025/12/21 21:54:37 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"
#include "utils.h"

static t_token	*new_token(t_token_kind tk, char *str, int len, t_flag flag)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_kind = tk;
	if (tk == TK_ERR || tk == TK_EOF || tk == TK_NEWLINE)
		return (token);
}

static t_lexeme	peek_lexeme(char *line)
{
	skip_space(&line);
	
}

t_token	*consume_lexeme(char **line, t_lexeme lexeme)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_kind = lexeme;
	if (tk == TK_ERR || tk == TK_EOF || tk == TK_NEWLINE)
		return (token);
}

static t_token	*generate_token(char **line)
{
	if (*line == NULL)
		return (new_token(TK_ERR, NULL, 0, 0));
	if (**line == '\0')
		return (new_token(TK_EOF, NULL, 0, 0));
	if ()
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
	while (1)
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
