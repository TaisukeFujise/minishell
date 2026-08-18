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
*/
static t_status	subshell_body(t_node *node, t_ctx *ctx)
{
	if (expand_command(node, ctx, ctx->arenas) != ST_OK
		|| apply_redirects(node->u_node.subshell.redirects, false) != ST_OK)
		exit(EXIT_FAILURE);
	return (execute_internal(node->left, ctx, false));
}

t_status	exec_subshell(t_node *node, t_ctx *ctx, bool own)
{
	pid_t	pid;

	if (own)
		return (subshell_body(node, ctx));
	pid = fork();
	if (pid < 0)
		return (ctx->err.exit_code = 1, ST_FAILURE);
	if (pid == 0)
	{
		reset_signals();
		subshell_body(node, ctx);
		exit(ctx->err.exit_code);
	}
	return (wait_pid_status(ctx, pid));
}
