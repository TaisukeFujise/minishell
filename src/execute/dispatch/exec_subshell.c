/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 22:38:06 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

t_status	exec_subshell(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ST_FATAL);
	else if (pid == 0)
	{
		ctx->subshell_level++;
		_exit(execute_internal(node, ctx, pipe_in, pipe_out));
	}
	else
	{
		ctx->already_forked = 1;
		close_pipes(pipe_in, pipe_out);
		return (register_pid(ctx, pid));
	}
	return (ST_FATAL);
}
