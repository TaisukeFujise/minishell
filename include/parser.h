/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:37 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:30:54 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

# define PARSER_MSG_SYNTAX "syntax error"
# define PARSER_MSG_FATAL "parser error"
# define PARSER_MSG_TOKEN_NEWLINE "newline"
# define PARSER_MSG_UNEXPECTED_PREFIX "syntax error near unexpected token `"
# define PARSER_MSG_UNCLOSED_SQ \
	"syntax error: unexpected EOF while looking for matching `''"
# define PARSER_MSG_UNCLOSED_DQ \
	"syntax error: unexpected EOF while looking for matching `\"'"
# define PARSER_MSG_UNCLOSED_SUBSHELL \
	"syntax error: unexpected EOF while looking for matching `)'"
# define PARSER_MSG_HD_EOF_PREFIX \
	"warning: here-document delimited by end-of-file (wanted `"
# define PARSER_MSG_HD_EOF_SUFFIX "')"
# define PARSER_MSG_HD_MAX "maximum here-document count exceeded"
# define HEREDOC_MAX 16

typedef enum e_node_kind
{
	NODE_COMPLETE,
	NODE_ANDOR,
	NODE_PIPE,
	NODE_SUBSHELL,
	NODE_SIMPLE
}	t_node_kind;

typedef struct s_heredoc
{
	t_word	raw_str;
	int		content_fd; // init value is -1
}	t_heredoc;

typedef struct s_redirect	t_redirect;

struct s_redirect
{
	t_op_redir	op;
	t_word		target; // filename or delimiter
	int			io_number;
	t_heredoc	hd;
	t_redirect	*next;
};

typedef struct s_simple_cmd
{
	t_assign	*assigns;
	t_word_list	*args;
	t_redirect	*redirects;
}	t_simple_cmd;

typedef struct s_subshell
{
	t_redirect	*redirects;
}	t_subshell;

typedef struct s_and_or
{
	t_op_connect	op;
}	t_and_or;

typedef struct s_node		t_node;

struct s_node
{
	t_node_kind	node_kind;
	t_node		*left;
	t_node		*right;
	union
	{
		t_and_or		and_or;
		t_subshell		subshell;
		t_simple_cmd	simple_command;
	}	u_node;
};

t_status	parse(char **cursor, t_node *ast, t_ctx *ctx, t_arenas *arenas);
void		close_heredocs(t_node *node);

#endif
