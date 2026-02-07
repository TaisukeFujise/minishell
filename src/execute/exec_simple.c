/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:15 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/07 00:20:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

bool	find_builtin(char *str);

/*
	Todo
	- Create tmp_env_table from "t_word_list *assigns"
	- Expand Environment Variable
	- Set Redirect from "t_redirect *redirects"
	- Execute cmd, which is "built-in" or "execve",
		using executor->input_fd and executor->output_fd.
	- Update last_pid in ctx in order to waitpid.
*/
t_status	exec_simple(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	ctx->already_forked = 0; // initialize ctx->already_forked flag.
	if (expand_words(node->u_node.simple_command, ctx)
		!= ST_OK)
		return (ST_FAILURE);
	if (node->u_node.simple_command.args == 0)
		return (exec_null_command(&node->u_node.simple_command,
				ctx, pipe_in, pipe_out));
	if (find_builtin(node->u_node.simple_command.args->wd->str))
		return (exec_builtin(&node->u_node.simple_command,
				ctx, pipe_in, pipe_out));
	return (exec_disk_command(&node->u_node.simple_command, ctx, pipe_in, pipe_out));
}

bool	find_builtin(char *str)
{
	if ((ft_strcmp(str, "echo") == 0)
		|| (ft_strcmp(str, "cd") == 0)
		|| (ft_strcmp(str, "pwd") == 0)
		|| (ft_strcmp(str, "export") == 0)
		|| (ft_strcmp(str, "export") == 0)
		|| (ft_strcmp(str, "unset")== 0)
		|| (ft_strcmp(str, "env") == 0)
		|| (ft_strcmp(str, "exit") == 0))
		return (true);
	return (false);
}
