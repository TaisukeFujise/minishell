/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:59:19 by fendo             #+#    #+#             */
/*   Updated: 2025/12/24 17:19:37 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"
#include "utils.h"

void	lex_control(char **line, t_token *token)
{
	if (ft_strncmp(*line, "\0", 1))
		token->token_kind = TK_EOF;
	else if (ft_strncmp(*line, "\n", 1))
	{
		token->token_kind = TK_NEWLINE;
		(*line)++;
	}
}

void	lex_connect(char **line, t_token *token)
{
	if (ft_strncmp(*line, "&&", 2))
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_AND_IF;
		(*line) += 2;
	}
	else if (ft_strncmp(*line, "||", 2))
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_OR_IF;
		(*line) += 2;
	}
	else if (ft_strncmp(*line, "|", 1))
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_PIPE;
		(*line)++;
	}
}

void	lex_group(char **line, t_token *token)
{
	if (ft_strncmp(*line, "(", 1))
	{
		token->token_kind = TK_GROUP;
		token->u_token.op_group = GROUP_LPAREN;
		(*line)++;
	}
	else if (ft_strncmp(*line, ")", 1))
	{
		token->token_kind = TK_GROUP;
		token->u_token.op_group = GROUP_RPAREN;
		(*line)++;
	}
}

void	lex_redirect(char **line, t_token *token)
{
	if (ft_strncmp(*line, ">>", 2))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_DGREAT;
		(*line) += 2;
	}
	else if (ft_strncmp(*line, "<<", 2))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_DLESS;
		(*line) += 2;
	}
	else if (ft_strncmp(*line, ">", 1))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_GREAT;
		(*line)++;
	}
	else if (ft_strncmp(*line, "<", 1))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_LESS;
		(*line)++;
	}
}

void	lex_io_number(char **line, t_token *token)
{
	char			*endptr;
	unsigned int	fd;

	endptr = *line;
	fd = str2fd(&endptr);
	if ((endptr != *line && ft_strchr("><", **line)))
	{
		token->token_kind = TK_IO_NUMBER;
		token->u_token.io_num = fd;
		(*line) = endptr;
	}
}
