/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:37 by fendo             #+#    #+#             */
/*   Updated: 2025/12/20 18:44:29 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef enum e_node_kind
{
	NODE_PROGRAM,
	NODE_COMPLETE,
	NODE_ANDOR,
	NODE_PIPE,
	NODE_SUBSHELL,
	NODE_SIMPLE
}	t_node_kind;

typedef struct s_redirect	t_redirect;

struct s_redirect
{
	t_op_redir	op;
	t_word_desc	target; // filename or delimiter
	int			fd;		// -1 if IO_NUMBER is not specified
	t_redirect	*next;
};

typedef struct s_simple_cmd
{
	t_word_list	*assigns;
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
	t_node		*child;
	t_node		*next;
	union
	{
		t_and_or		and_or;
		t_subshell		subshell;
		t_simple_cmd	simple_command;
	}	u_cmd;
};

#endif
