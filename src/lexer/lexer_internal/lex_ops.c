/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:59:19 by fendo             #+#    #+#             */
/*   Updated: 2026/02/03 20:20:40 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

t_token_kind	lex_control(char **line, t_token *token)
{
	if (!ft_strncmp(*line, "\0", 1))
		token->token_kind = TK_EOF;
	else if (!ft_strncmp(*line, "\n", 1))
	{
		token->token_kind = TK_NEWLINE;
		token->u_token.nl_ptr = *line;
		(*line)++;
	}
	return (token->token_kind);
}

t_token_kind	lex_connect(char **line, t_token *token)
{
	if (!ft_strncmp(*line, "&&", 2))
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_AND_IF;
		(*line) += 2;
	}
	else if (!ft_strncmp(*line, "||", 2))
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_OR_IF;
		(*line) += 2;
	}
	else if (!ft_strncmp(*line, "|", 1))
	{
		token->token_kind = TK_CONNECT;
		token->u_token.op_connect = CONNECT_PIPE;
		(*line)++;
	}
	return (token->token_kind);
}

t_token_kind	lex_group(char **line, t_token *token, t_lex_state *st)
{
	if (!ft_strncmp(*line, "(", 1))
	{
		token->token_kind = TK_GROUP;
		token->u_token.op_group = GROUP_LPAREN;
		st->paren_depth++;
		(*line)++;
	}
	else if (!ft_strncmp(*line, ")", 1))
	{
		if (st->paren_depth == 0)
			set_lex_error(token, ERR_UNCLOSED_SUBSHELL);
		else
		{
			token->token_kind = TK_GROUP;
			token->u_token.op_group = GROUP_RPAREN;
			st->paren_depth--;
		}
		(*line)++;
	}
	return (token->token_kind);
}

t_token_kind	lex_redirect(char **line, t_token *token)
{
	if (!ft_strncmp(*line, ">>", 2))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_DGREAT;
		(*line) += 2;
	}
	else if (!ft_strncmp(*line, "<<", 2))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_DLESS;
		(*line) += 2;
	}
	else if (!ft_strncmp(*line, ">", 1))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_GREAT;
		(*line)++;
	}
	else if (!ft_strncmp(*line, "<", 1))
	{
		token->token_kind = TK_REDIR;
		token->u_token.op_redir = REDIR_LESS;
		(*line)++;
	}
	return (token->token_kind);
}

t_token_kind	lex_io_number(char **line, t_token *token)
{
	char			*end_ptr;
	unsigned int	fd;

	end_ptr = *line;
	fd = str2fd(&end_ptr);
	if (end_ptr != *line && ft_strchr("><", *end_ptr))
	{
		token->token_kind = TK_IO_NUMBER;
		token->u_token.io_num = fd;
		(*line) = end_ptr;
	}
	return (token->token_kind);
}
