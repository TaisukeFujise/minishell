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
#include "../../../include/signal_handle.h"
#include "../../../include/expand.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	The body of a subshell runs in a process of its own. When this
	process is already one, it is taken over instead of forking again,
	the way dash skips the fork under EV_EXIT.
	The prefix assignments of the command before it are not this
	command's, so they are dropped here as well: every caller of
	expand_command() flushes first. [review OVL-01]
*/
static t_status	subshell_body(t_node *node, t_ctx *ctx)
{
	hash_flush(ctx->tmp_table, NULL);
	if (expand_command(node, ctx, ctx->arenas) != ST_OK
		|| apply_redirects(node->u_node.subshell.redirects,
			REDIR_KEEP) != ST_OK)
		exit(EXIT_FAILURE);
	return (execute_internal(node->left, ctx, EXEC_MAY_FORK));
}

t_status	exec_subshell(t_node *node, t_ctx *ctx, t_exec_mode mode)
{
	pid_t	pid;

	if (mode == EXEC_NO_FORK)
		return (subshell_body(node, ctx));
	pid = fork();
	if (pid < 0)
		return (ctx->err.exit_code = 1, ST_FAILURE);
	if (pid == 0)
	{
		reset_signals();
		ctx->interactive = false;
		subshell_body(node, ctx);
		exit(ctx->err.exit_code);
	}
	return (wait_pid_status(ctx, pid));
}
