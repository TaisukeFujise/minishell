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
	- copy the fd bitmap for the stages, with the read end added
	- run the left hand into the write end, the right hand out of the
	  read end, and close this pipe on the way out
	The endpoints belong here: no stage closes them for us. A setup
	failure stops the pipeline instead of being hidden by the right hand.
	[review P41-05, P41-07, P41-10]
*/
static t_status	run_stages(t_node *node, t_ctx *ctx, t_stage st)
{
	int			pipe_fd[2];
	t_stage		side;
	t_status	result;

	if (pipe(pipe_fd) < 0)
		return (ctx->err.exit_code = 1, ST_FAILURE);
	side = st;
	side.close = grow_fd_bitmap(st.close, pipe_fd[0]);
	if (side.close == NULL)
		return (close(pipe_fd[0]), close(pipe_fd[1]), ST_FATAL);
	side.close->bitmap[pipe_fd[0]] = 1;
	side.pipe_out = pipe_fd[1];
	result = execute_internal(node->left, ctx, side);
	close(pipe_fd[1]);
	if (result == ST_OK)
	{
		side.pipe_in = pipe_fd[0];
		side.pipe_out = st.pipe_out;
		result = execute_internal(node->right, ctx, side);
	}
	close(pipe_fd[0]);
	dispose_fd_bitmap(side.close);
	return (result);
}

/*
	The outermost pipeline node owns the process set of the whole
	pipeline: the stages of "a | b | c" are two nested nodes but one set,
	and they must all be started before any of them is waited for.
*/
t_status	exec_pipeline(t_node *node, t_ctx *ctx, t_stage st)
{
	t_procs		procs;
	t_status	result;

	if (st.procs != NULL)
		return (run_stages(node, ctx, st));
	if (!procs_init(&procs, count_stages(node)))
		return (ST_FATAL);
	st.procs = &procs;
	result = run_stages(node, ctx, st);
	if (result == ST_OK)
		return (procs_wait(&procs, ctx));
	return (procs_wait(&procs, ctx), result);
}
