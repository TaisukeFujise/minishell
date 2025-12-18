/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:00:37 by fendo             #+#    #+#             */
/*   Updated: 2025/12/17 03:15:39 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_redirect
{
	t_op		op;
	t_word_desc	target; // filename or delimiter
	int			fd;
}	t_redirect;

typedef struct s_simple_cmd
{
	t_word_desc	*assigns;
	t_word_desc	*args;
	t_redirect	*redirects;
}	t_simple_cmd;

typedef struct s_program t_program;

typedef struct s_subshell
{
	t_program	*program;
	t_redirect	*redirects;
}	t_subshell;

typedef enum e_cmd_kind
{
	CMD_SIMPLE,
	CMD_SUBSHELL
}	t_cmd_kind;

typedef struct s_command
{
	t_cmd_kind	cmd_kind;
	union
	{
		t_simple_cmd	simple;
		t_subshell		subshell;
	};
}	t_command;

typedef struct s_pipeline
{
	t_command	*cmds;
}	t_pipeline;

typedef struct s_and_or
{
	t_pipeline	*pipes;
	t_op		*ops;
}	t_and_or;

typedef struct s_complete_commands
{
	t_and_or	*and_ors;
}	t_complete_commands;

typedef struct s_program
{
	t_complete_commands	comp_cmds;
}	t_program;

#endif
