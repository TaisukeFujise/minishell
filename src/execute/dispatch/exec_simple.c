/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:15 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 21:25:21 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../../../include/execute.h"
#include "../../../include/expand.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	Todo
	- Create tmp_env_table from "t_word_list *assigns"
	- Set Redirect from "t_redirect *redirects"
	- Execute cmd, which is "built-in" or "execve",
		using executor->input_fd and executor->output_fd.
	- Update last_pid in ctx in order to waitpid.
*/
t_status	exec_simple(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_simple_cmd	*cmd;
	t_status		status;

	hash_flush(ctx->tmp_table, NULL);
	status = expand_command(node, ctx, ctx->arenas);
	if (status != ST_OK)
		return (status);
	cmd = &node->u_node.simple_command;
	if (cmd->args != NULL && find_builtin(cmd->args->wd->str) == NULL)
		return (exec_disk_command(cmd, ctx, pipe_in, pipe_out));
	return (exec_builtin(cmd, ctx, pipe_in, pipe_out));
}
