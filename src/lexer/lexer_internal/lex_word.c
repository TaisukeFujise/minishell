/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:43:37 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:43:15 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "../../../include/minishell.h"

/*
	parse.y ref:
	- read_token_word: single quote scanning (L5385-L5415)
	lex rule:
	- <SQ> [^']+ -> part(W_SQ)
	- <SQ> "'"   -> end quote, return to <WORD>
	- <SQ> "\0"  -> TK_ERR(UNCLOSED_SINGLE_QUOTE)
*/
static t_lexer_err	scan_sq(char **line, t_word_builder *wb)
{
	char	*begin;

	begin = ++(*line);
	ft_strspn(line, "\'", false);
	if (**line != '\'')
		return (LEX_ERR_UNCLOSED_SINGLE_QUOTE);
	return (close_quote_part(wb, line, begin, W_SQ));
}

/*
	parse.y ref:
	- read_token_word: double quote scanning (L5417-L5503)
	lex rule:
	- <DQ> \$NAME   -> part(W_DQ|W_DOLL)
	- <DQ> \$\?       -> part(W_DQ|W_DOLL)
	- <DQ> \$         -> part(W_DQ)  (bare dollar)
	- <DQ> [^"$\0]+   -> part(W_DQ)
	- <DQ> '"'        -> end quote, return to <WORD>
	- <DQ> "\0"       -> TK_ERR(UNCLOSED_DOUBLE_QUOTE)
*/
static t_lexer_err	scan_dq(char **line, t_word_builder *wb)
{
	char	*begin;
	uint8_t	flag;
	uint8_t	dollar_flag;

	begin = ++(*line);
	while (**line && **line != '\"')
	{
		flag = W_DQ;
		if (**line == '$')
		{
			dollar_flag = W_NONE;
			lex_dollar(line, &dollar_flag);
			flag |= dollar_flag;
		}
		else
			ft_strspn(line, "\"$", false);
		if (commit_part(wb, &begin, *line, flag) != LEX_NO_ERR)
			return (LEX_ERR_MEMORY_ALLOCATION);
	}
	if (**line != '\"')
		return (LEX_ERR_UNCLOSED_DOUBLE_QUOTE);
	return (close_quote_part(wb, line, begin, W_DQ));
}

/*
	parse.y ref:
	- read_token_word: '$' handling (L5507-L5567), default char (L5700+)
	lex rule:
	- <WORD> \$NAME  -> part(W_DOLL)
	- <WORD> \$\?      -> part(W_DOLL)
	- <WORD> \$        -> part(W_NONE)
	- <WORD> \*+       -> part(W_WILD)
	- <WORD> PLAIN+  -> part(W_NONE)
*/
static t_lexer_err	scan_unquoted(char **line, t_word_builder *wb,
									t_assign_info *as)
{
	char	*begin;
	uint8_t	flag;

	begin = *line;
	flag = W_NONE;
	if (**line == '$')
		lex_dollar(line, &flag);
	else if (**line == '*')
	{
		flag = W_WILD;
		ft_strspn(line, "*", true);
	}
	else
		while (**line && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
			validate_assign((*line)++, as);
	return (commit_part(wb, &begin, *line, flag));
}

/*
	parse.y ref:
	- read_token_word: main loop (L5345-L5700)
	lex rule:
	- <WORD> dispatch: scan_sq / scan_dq / scan_unquoted
	- <WORD> BOUND|DBOUND|\0 -> emit TK_WORD, BEGIN(INITIAL)
*/
static t_lexer_err	scan_word(char **line, t_word **head,
								t_assign_info *as, t_arena *arena)
{
	t_word_builder	wb;
	t_lexer_err		err;

	wb.arena = arena;
	wb.tail = head;
	while (**line && !is_tk_bound(*line))
	{
		if (ft_strchr("\'\"$*", **line))
			validate_assign(*line, as);
		if (**line == '\'')
			err = scan_sq(line, &wb);
		else if (**line == '\"')
			err = scan_dq(line, &wb);
		else
			err = scan_unquoted(line, &wb, as);
		if (err)
			return (err);
	}
	return (LEX_NO_ERR);
}

/*
	parse.y ref:
	- read_token_word: entry + got_token (L5305, L5720-L5741)
	lex rule:
	- <INITIAL> -> BEGIN(WORD) -> emit TK_WORD
	- apply_assign_info: Apply the result of side-channel FSM
*/
t_token_kind	lex_word(char **line, t_token *tk, t_arena *arena)
{
	t_word			*head;
	t_assign_info	as;
	t_lexer_err		err;

	head = NULL;
	set_assign_info(&as, AS_INIT, NULL, W_NONE);
	err = scan_word(line, &head, &as, arena);
	if (err)
	{
		if (err != LEX_ERR_MEMORY_ALLOCATION)
			set_lexer_error(tk, err);
		return (tk->token_kind);
	}
	tk->token_kind = TK_WORD;
	tk->u_token.wd = head;
	apply_assign_info(head, &as);
	return (tk->token_kind);
}
