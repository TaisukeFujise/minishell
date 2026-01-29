/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/29 17:49:54 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

/*
	(ctx->bitmapのinitをinit_ctxへ)
	execute init fd_bitmap and wrap execute_internal.
	after the func call, dispose the fd_bitmap, and return t_status result.
*/
// t_status	execute(t_node *root, t_ctx *ctx)
// {
// 	t_status	result;

// 	if (root == NULL)
// 		return (ST_OK);// parse error passes NULL root ast.
// 	ctx->bitmap = new_fd_bitmap(FD_BITMAP_SIZE);
// 	result = execute_internal(root, ctx, NO_PIPE, NO_PIPE);
// 	dispose_fd_bitmap(ctx->bitmap);
// 	return (result);
// }

/*
	execute call exec_*** by the type of node, and return t_status result.

	- exec_subshell
	- exec_simple
	- exec_connection
*/
t_status	execute(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;

	if (node == NULL)
		return (ST_OK);
	if (node->node_kind == NODE_SUBSHELL)
	{
		result = exec_subshell(node, ctx, pipe_in, pipe_out);
		if (pipe_in != NO_PIPE && pipe_out != NO_PIPE)
			wait_pids(ctx);
	}
	else if (node->node_kind == NODE_SIMPLE)
	{
		result = exec_simple(node, ctx, pipe_in, pipe_out);
		if (ctx->already_forked && pipe_out == NO_PIPE)
			wait_pids(ctx);
	}
	else if (node->node_kind == NODE_COMPLETE || NODE_ANDOR || NODE_PIPE)
		result = exec_connection(node, ctx, pipe_in, pipe_out);
	else
		result = ST_FATAL;
	return (result);
}
