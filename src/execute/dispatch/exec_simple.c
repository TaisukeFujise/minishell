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
t_status	exec_simple(t_node *node, t_ctx *ctx, t_stage st)
{
	t_simple_cmd	*cmd;
	t_status		status;

	hash_flush(ctx->tmp_table, NULL);
	status = expand_command(node, ctx, ctx->arenas);
	cmd = &node->u_node.simple_command;
	if (status == ST_OK)
	{
		if (cmd->args != NULL && find_builtin(cmd->args->wd->str) == NULL)
			status = exec_disk_command(cmd, ctx, st);
		else
			status = exec_builtin(cmd, ctx, st);
	}
	if (status == ST_OK)
		return (ST_OK);
	return (set_exit_code(ctx, status));
}

/*
	The numeric status of a command comes from the command: from the
	builtin that ran, or from waiting for the child. This settles the
	status of what happens around it instead: an expansion, a fork or a
	redirect the shell could not carry out is a plain failure, and an
	internal failure that chose no code must not read as success.
	[review D37-12, D37-17]
*/
t_status	set_exit_code(t_ctx *ctx, t_status status)
{
	if (status == ST_OK)
		ctx->err.exit_code = 0;
	else if (status == ST_FAILURE)
		ctx->err.exit_code = 1;
	else if (status == ST_FATAL && ctx->err.exit_code == 0)
		ctx->err.exit_code = 1;
	return (status);
}
