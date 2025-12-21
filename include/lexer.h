/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:33 by fendo             #+#    #+#             */
/*   Updated: 2025/12/21 22:29:04 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef enum e_token_kind
{
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
		t_word_desc		wd;
		t_op_connect	op_bin;
		t_op_group		op_group;
		t_op_redir		op_redir;
		int				io_num;
		int				err;
	}	u_token;
};

t_token	*tokenize(char *line);
void	free_tokens(t_token *head);

#endif
