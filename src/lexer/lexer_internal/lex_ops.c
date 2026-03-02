/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:59:19 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:42:17 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "lexer_internal.h"

/*
	parse.y ref:
	- read_token: '\n' return (L3657), yacc_EOF (L3618-L3620)
	lex rule:
	- <INITIAL> "\0" -> TK_EOF  (paren_depth != 0 -> TK_ERR)
	- <INITIAL> "\n" -> TK_NEWLINE
*/
t_token_kind	lex_control(char **line, t_token *token)
{
	if (**line == '\0')
		token->token_kind = TK_EOF;
	else if (**line == '\n')
	{
		token->token_kind = TK_NEWLINE;
		(*line)++;
	}
	return (token->token_kind);
}

/*
	parse.y ref:
	- read_token: AND_AND/OR_OR dispatch (L3715-L3723), '|' (L3814)
	lex rule:
	- <INITIAL> "&&" -> TK_CONNECT(AND_IF)
	- <INITIAL> "||" -> TK_CONNECT(OR_IF)
	- <INITIAL> "|"  -> TK_CONNECT(PIPE)
*/
t_token_kind	lex_connect(char **line, t_token *token)
{
	if (ft_strncmp(*line, "&&", 2) == 0)
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_AND_IF;
		(*line) += 2;
	}
	else if (ft_strncmp(*line, "||", 2) == 0)
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_OR_IF;
		(*line) += 2;
	}
	else if (**line == '|')
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_PIPE;
		(*line)++;
	}
	return (token->token_kind);
}

/*
	parse.y ref:
	- read_token: '(' / ')' handling (L3781-L3810)
	lex rule:
	- <INITIAL> "(" -> TK_GROUP(LPAREN)   paren_depth++
	- <INITIAL> ")" -> TK_GROUP(RPAREN)   paren_depth-- (>0)
*/
t_token_kind	lex_group(char **line, t_token *token, t_lex_state *st)
{
	if (**line == '(')
	{
		token->token_kind = TK_GROUP;
		token->u_token.op_group = GROUP_LPAREN;
		st->paren_depth++;
		(*line)++;
	}
	else if (**line == ')')
	{
		token->token_kind = TK_GROUP;
		token->u_token.op_group = GROUP_RPAREN;
		if (st->paren_depth > 0)
			st->paren_depth--;
		(*line)++;
	}
	return (token->token_kind);
}

/*
	parse.y ref:
	- read_token: LESS_LESS/GREATER_GREATER (L3697-L3713),
	  single '<' / '>' (L3725-L3762)
	lex rule:
	- <INITIAL> "<<" -> TK_REDIR(DLESS)
	- <INITIAL> ">>" -> TK_REDIR(DGREATER)
	- <INITIAL> "<"  -> TK_REDIR(LESS)
	- <INITIAL> ">"  -> TK_REDIR(GREATER)
*/
t_token_kind	lex_redirect(char **line, t_token *token)
{
	if (ft_strncmp(*line, ">>", 2) == 0)
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_DGREATER;
		(*line) += 2;
	}
	else if (ft_strncmp(*line, "<<", 2) == 0)
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_DLESS;
		(*line) += 2;
	}
	else if (**line == '>')
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_GREATER;
		(*line)++;
	}
	else if (**line == '<')
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_LESS;
		(*line)++;
	}
	return (token->token_kind);
}

/*
	parse.y ref:
	- read_token_word: all_digit_token + NUMBER (L5731-L5741)
	lex rule:
	- <INITIAL> DIGIT+/[<>] -> TK_IO_NUMBER (value <= INT_MAX)
*/
t_token_kind	lex_io_number(char **line, t_token *token)
{
	char			*end_ptr;
	int				fd;

	end_ptr = *line;
	fd = str2fd(&end_ptr);
	if (end_ptr != *line && ft_strchr("><", *end_ptr) && fd >= 0)
	{
		token->token_kind = TK_IO_NUMBER;
		token->u_token.io_num = fd;
		(*line) = end_ptr;
	}
	return (token->token_kind);
}
