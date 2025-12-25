/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:44:56 by fendo             #+#    #+#             */
/*   Updated: 2025/12/24 19:07:41 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"
#include "utils.h"

static void	case_single_quote(char **line, uint16_t *state, t_token *token)
{
	if (ft_strncmp(*line, "\'", 1) == 0)
	{
		*state = W_NONE;
		token->u_token.wd.flag |= W_SQ;
	}
	(*line)++;
}

static void	case_double_quote(char **line, uint16_t *state, t_token *token)
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

static void	case_none(char **line, uint16_t *state, t_token *token)
{
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

static void	set_word_token(char *beginptr, char *endptr,
		uint16_t state, t_token *token)
{
	if (state != W_NONE)
	{
		token->token_kind = TK_ERR;
		token->u_token.err = ERR_UNCLOSED_QUOTE;
		return ;
	}
	token->token_kind = TK_WORD;
	token->u_token.wd.word.str = beginptr;
	token->u_token.wd.word.len = endptr - beginptr;
}

void	lex_word(char **line, t_token *token)
{
	char		*beginptr;
	uint16_t	state;

	beginptr = *line;
	state = W_NONE;
	while (**line != '\0')
	{
		if (state == W_SQ)
			case_single_quote(line, &state, token);
		else if (state == W_DQ)
			case_double_quote(line, &state, token);
		else
		{
			if ((ft_strchr(" \t\n|()><", **line)
					|| strchunk("&&||", *line, 2)))
				break ;
			case_none(line, &state, token);
		}
	}
	set_word_token(beginptr, *line, state, token);
}
