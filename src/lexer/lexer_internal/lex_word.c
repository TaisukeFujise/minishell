/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:44:56 by fendo             #+#    #+#             */
/*   Updated: 2025/12/27 16:58:16 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

static void	_case_single_quote(char **line, uint8_t *state, t_token *token)
{
	if (ft_strncmp(*line, "\'", 1) == 0)
	{
		*state = W_NONE;
		token->u_token.wd.flag |= W_SQ;
	}
	(*line)++;
}

static void	_case_double_quote(char **line, uint8_t *state, t_token *token)
{
	if (ft_strncmp(*line, "\"", 1) == 0)
	{
		*state = W_NONE;
		token->u_token.wd.flag |= W_DQ;
	}
	else if (ft_strncmp(*line, "$", 1) == 0)
		token->u_token.wd.flag |= W_DOLL;
	(*line)++;
	return ;
}

static void	_case_none(char **line, uint8_t *state,
			t_token *token, t_assign_state *as)
{
	validate_assign(*line, token, as);
	if (ft_strncmp(*line, "\'", 1) == 0)
		*state = W_SQ;
	else if (ft_strncmp(*line, "\"", 1) == 0)
		*state = W_DQ;
	else if (ft_strncmp(*line, "$", 1) == 0)
		token->u_token.wd.flag |= W_DOLL;
	else if (ft_strncmp(*line, "*", 1) == 0)
		token->u_token.wd.flag |= W_WILD;
	(*line)++;
}

static void	_set_word_token(char *begin_ptr, char *end_ptr,
		uint8_t state, t_token *token)
{
	if (state != W_NONE)
	{
		token->token_kind = TK_ERR;
		token->u_token.err = ERR_UNCLOSED_QUOTE;
		return ;
	}
	token->token_kind = TK_WORD;
	token->u_token.wd.word.str = begin_ptr;
	token->u_token.wd.word.len = end_ptr - begin_ptr;
}

void	lex_word(char **line, t_token *token)
{
	char				*begin_ptr;
	uint8_t				state;
	t_assign_state		as;

	begin_ptr = *line;
	state = W_NONE;
	as = AS_INIT;
	token->u_token.wd.flag = W_NONE;
	token->u_token.wd.eq_ptr = NULL;
	while (**line != '\0')
	{
		if (state == W_SQ)
			_case_single_quote(line, &state, token);
		else if (state == W_DQ)
			_case_double_quote(line, &state, token);
		else
		{
			if ((ft_strchr(" \t\n|()><", **line)
					|| strchunk("&&||>><<", *line, 2)))
				break ;
			_case_none(line, &state, token, &as);
		}
	}
	_set_word_token(begin_ptr, *line, state, token);
}
