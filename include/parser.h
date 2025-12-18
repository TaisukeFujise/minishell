/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:37 by fendo             #+#    #+#             */
/*   Updated: 2025/12/18 17:43:17 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef enum e_node_kind
{
	NODE_PROGRAM,
	NODE_COMPS,
	NODE_ANDOR,
	NODE_PIPE,
	NODE_SUBSHELL,
	NODE_SIMPLE
}	t_node_kind;

typedef struct s_redirect
{
	t_redir		op;
	t_word_desc	target; // filename or delimiter
	int			fd;
}	t_redirect;

typedef struct s_simple_cmd
{
	t_word_desc	*assigns;
	t_word_desc	*args;
	t_redirect	*redirects;
}	t_simple_cmd;

typedef struct s_subshell
{
	t_redirect	*redirects;
}	t_subshell;

typedef struct s_and_or
{
	t_connect	*ops;
}	t_and_or;

typedef struct s_node
{
	t_node_kind	node_kind;
	t_node		*children;
	union
	{
		t_and_or		and_or;
		t_subshell		subshell;
		t_simple_cmd	simple_command;
	};
}	t_node;

#endif
