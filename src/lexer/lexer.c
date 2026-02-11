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
#include <stdbool.h>

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

t_token	*free_tokens(t_token *head, t_token *extra)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->token_kind == TK_WORD)
			free_word_parts(head->u_token.wd);
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
	t_token			*head;
	t_token			**tail;
	t_token			*token;
	t_lex_state		st;
	t_token_kind	tk_kind;

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
		tk_kind = lexer_step(&line, token, &st);
		if (tk_kind == TK_UNSET)
			return (free_tokens(head, token));
		if (tk_kind == TK_ERR)
		{
			free_tokens(head, NULL);
			return (token);
		}
		(*tail) = token;
		tail = &token->next;
		if (tk_kind == TK_EOF)
			break ;
	}
	return (head);
}
