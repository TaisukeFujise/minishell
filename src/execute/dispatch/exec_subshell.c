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
#include "../../../include/expand.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

t_status	exec_subshell(t_node *node, t_ctx *ctx, t_stage st)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ST_FATAL);
	if (pid == 0)
	{
		free(ctx->pids);
		ctx->pids = NULL;
		ctx->npid = 0;
		enter_child(st);
		if (expand_command(node, ctx, ctx->arenas) != ST_OK
			|| apply_redirects(node->u_node.subshell.redirects, false) != ST_OK)
			exit(EXIT_FAILURE);
		st.pipe_in = NO_PIPE;
		st.pipe_out = NO_PIPE;
		execute_internal(node->left, ctx, st);
		exit(ctx->err.exit_code);
	}
	close_pipes(st);
	return (register_pid(ctx, pid));
}
