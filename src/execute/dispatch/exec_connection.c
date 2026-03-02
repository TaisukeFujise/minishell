/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_connection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:17 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 21:42:48 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

t_status	exec_connection(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	if (node->node_kind == NODE_COMPLETE)
		return (exec_complete(node, ctx, pipe_in, pipe_out));
	if (node->node_kind == NODE_ANDOR)
		return (exec_andor(node, ctx, pipe_in, pipe_out));
	if (node->node_kind == NODE_PIPE)
		return (exec_pipeline(node, ctx, pipe_in, pipe_out));
	return (ST_FATAL);
}

t_status	exec_complete(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	execute_internal(node->left, ctx, pipe_in, pipe_out);
	return (execute_internal(node->right, ctx, pipe_in, pipe_out));
}

t_status	exec_andor(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;

	result = execute_internal(node->left, ctx, pipe_in, pipe_out);
	if (node->u_node.and_or.op == CONNECT_AND_IF)
	{
		if (result == ST_OK)
			return (execute_internal(node->right, ctx, pipe_in, pipe_out));
		return (ST_FAILURE);
	}
	else
	{
		if (result != ST_OK)
			return (execute_internal(node->right, ctx, pipe_in, pipe_out));
		return (ST_FAILURE);
	}
}

/*
	- make pipe
	- recreate fd_bitmap and register pipe_fd[0] to fd_bitmap.

	- set pipe_in and pipe_fd[1] to left hand (register pipe_fd[0] to fd_bitmap)
	- set pipe_fd[0] and pipe_out to right hand.
*/
t_status	exec_pipeline(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	int			pipe_fd[2];
	t_fd_bitmap		*fd_bitmap;
	t_status	result;

	if (pipe(pipe_fd) < 0)
		return (ST_FAILURE);
	if (pipe_fd[0] < ctx->bitmap->size)
		fd_bitmap = new_fd_bitmap(ctx->bitmap->size);
	else
		fd_bitmap = new_fd_bitmap(pipe_fd[0] + 8);
	if (fd_bitmap == NULL)
		return (ST_FAILURE);
	ft_memcpy(fd_bitmap->bitmap, ctx->bitmap->bitmap, ctx->bitmap->size);
	dispose_fd_bitmap(ctx->bitmap);
	fd_bitmap->bitmap[pipe_fd[0]] = 1;
	ctx->bitmap = fd_bitmap;
	execute_internal(node->left, ctx, pipe_in, pipe_fd[1]);
	close(pipe_fd[1]);
	result = execute_internal(node->right, ctx, pipe_fd[0], pipe_out);
	close(pipe_fd[0]);
	return (result);
}
