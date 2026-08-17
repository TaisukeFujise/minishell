/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_connection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:17 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:40:23 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

t_status	exec_connection(t_node *node, t_ctx *ctx, t_stage st)
{
	if (node->node_kind == NODE_COMPLETE)
		return (exec_complete(node, ctx, st));
	if (node->node_kind == NODE_ANDOR)
		return (exec_andor(node, ctx, st));
	if (node->node_kind == NODE_PIPE)
		return (exec_pipeline(node, ctx, st));
	return (ST_FATAL);
}

t_status	exec_complete(t_node *node, t_ctx *ctx, t_stage st)
{
	t_status	result;

	result = execute_internal(node->left, ctx, st);
	if (result == ST_EXIT || result == ST_FATAL)
		return (result);
	if (node->right == NULL)
		return (result);
	return (execute_internal(node->right, ctx, st));
}

/*
	&& and || look at the status of the left hand command, which is the
	number it left in ctx->err.exit_code. A t_status only says whether
	the shell can go on.
*/
t_status	exec_andor(t_node *node, t_ctx *ctx, t_stage st)
{
	t_status	result;

	result = execute_internal(node->left, ctx, st);
	if (result == ST_EXIT || result == ST_FATAL)
		return (result);
	if (node->u_node.and_or.op == CONNECT_AND_IF && ctx->err.exit_code != 0)
		return (result);
	if (node->u_node.and_or.op == CONNECT_OR_IF && ctx->err.exit_code == 0)
		return (result);
	return (execute_internal(node->right, ctx, st));
}

/*
	- make pipe
	- recreate fd_bitmap and register pipe_fd[0] to fd_bitmap.

	- set pipe_in and pipe_fd[1] to left hand (register pipe_fd[0] to fd_bitmap)
	- set pipe_fd[0] and pipe_out to right hand.
*/
t_status	exec_pipeline(t_node *node, t_ctx *ctx, t_stage st)
{
	int			pipe_fd[2];
	t_fd_bitmap	*inner;
	t_stage		side;
	t_status	result;

	if (pipe(pipe_fd) < 0)
		return (ST_FAILURE);
	inner = grow_fd_bitmap(st.close, pipe_fd[0]);
	if (inner == NULL)
		return (close(pipe_fd[0]), close(pipe_fd[1]), ST_FAILURE);
	inner->bitmap[pipe_fd[0]] = 1;
	side = st;
	side.close = inner;
	side.out = pipe_fd[1];
	execute_internal(node->left, ctx, side);
	close(pipe_fd[1]);
	side.out = st.out;
	side.in = pipe_fd[0];
	result = execute_internal(node->right, ctx, side);
	close(pipe_fd[0]);
	dispose_fd_bitmap(inner);
	return (result);
}
