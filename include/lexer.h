/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:33 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:30:21 by fendo            ###   ########.fr       */
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
typedef struct s_lex_state	t_lex_state;

struct s_token
{
	t_token_kind	token_kind;
	union
	{
		t_word			*wd;
		t_op_connect	op_connect;
		t_op_group		op_group;
		t_op_redir		op_redir;
		int				io_num;
		t_lexer_err		err;
	}	u_token;
	const char		*lexeme_begin;
	size_t			lexeme_len;
};

struct s_lex_state
{
	char	*line;
	int		paren_depth;
	t_arena	*arena;
};

void			init_lex_state(t_lex_state *st, char *line, t_arena *arena);
t_status		tokenize(t_lex_state *st, t_token *token);
void			sync_next_line(t_lex_state *st);
void			free_token(t_token *token);

#endif
