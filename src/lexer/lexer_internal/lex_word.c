/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:44:56 by fendo             #+#    #+#             */
/*   Updated: 2025/12/26 22:48:57 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"
#include "utils.h"

static void	case_single_quote(char **line, uint8_t *state, t_token *token)
{
	if (ft_strncmp(*line, "\'", 1) == 0)
	{
		*state = W_NONE;
		token->u_token.wd.flag |= W_SQ;
	}
	(*line)++;
}

static void	case_double_quote(char **line, uint8_t *state, t_token *token)
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

static void	case_none(char **line, uint8_t *state, t_token *token, uint8_t *as)
{
	scan_asgn(**line, token, as);
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
		uint8_t state, t_token *token)
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
	uint8_t		state;
	uint8_t		as;

	beginptr = *line;
	state = W_NONE;
	token->u_token.wd.flag = W_NONE;
	as = AS_INIT;
	while (**line != '\0')
	{
		if (state == W_SQ)
			case_single_quote(line, &state, token);
		else if (state == W_DQ)
			case_double_quote(line, &state, token);
		else
		{
			if ((ft_strchr(" \t\n|()><", **line)
					|| strchunk("&&||>><<", *line, 2)))
				break ;
			case_none(line, &state, token, &as);
		}
	}
	set_word_token(beginptr, *line, state, token);
}
