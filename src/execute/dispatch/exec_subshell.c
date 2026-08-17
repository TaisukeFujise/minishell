/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:24:41 by tafujise         ###   ########.fr       */
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
	if (pid == 0)
	{
		execute_internal(node->left, ctx, pipe_in, pipe_out);
		_exit(ctx->err.exit_code);
	}
	return (adopt_child(ctx, pid, pipe_in, pipe_out));
}
