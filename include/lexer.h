/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:33 by fendo             #+#    #+#             */
/*   Updated: 2026/01/31 20:24:50 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

# define ERR_UNCLOSED_QUOTE 1

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

typedef struct s_token	t_token;

struct s_token
{
	t_token_kind	token_kind;
	t_token			*next;
	union
	{
		t_word			wd;
		t_op_connect	op_connect;
		t_op_group		op_group;
		t_op_redir		op_redir;
		int				io_num;
		char			*nl_ptr;
		int				err;
	}	u_token;
};

t_token	*tokenize(char *line);
void	free_tokens(t_token *head);

#endif
