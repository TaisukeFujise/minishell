/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:33 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 22:40:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef enum e_token_kind
{
	TK_UNSET,
	TK_WORD,
	TK_CONNECT,
	TK_GROUP,
	TK_REDIR,
	TK_IO_NUMBER,
	TK_NEWLINE,
	TK_EOF,
	TK_ERR
}	t_token_kind;

typedef enum e_lexer_err
{
	LEX_NO_ERR,
	LEX_ERR_UNCLOSED_SINGLE_QUOTE,
	LEX_ERR_UNCLOSED_DOUBLE_QUOTE,
	LEX_ERR_UNCLOSED_SUBSHELL,
	LEX_ERR_MEMORY_ALLOCATION = -1
}	t_lexer_err;

typedef struct s_token	t_token;

struct s_token
{
	t_token_kind	token_kind;
	t_token			*next;
	union
	{
		t_word			*wd;
		t_op_connect	op_connect;
		t_op_group		op_group;
		t_op_redir		op_redir;
		int				io_num;
		char			*nl_ptr;
		t_lexer_err		err;
	}	u_token;
};

t_token	*tokenize(char *line);
t_token	*free_tokens(t_token *head, t_token *extra);

#endif
