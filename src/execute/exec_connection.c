/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_connection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:17 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/28 00:13:20 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

t_status	exec_complete(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{

	// while (node != NULL)
	// {
	// 	result = execute(node->child, executor, ctx);
	// 	if (result == ST_FATAL)
	// 		return (ST_FATAL);
	// 	node = node->next;
	// }
	// return (result);
}

t_status	exec_andor(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{

	// while (node != NULL)
	// {
	// 	result = execute(node->child, executor, ctx);
	// 	if (result == ST_FATAL)
	// 		return (ST_FATAL);
	// 	if (node->u_node.and_or.op == CONNECT_AND_IF)
	// 		if (ctx->exit_code != 0)
	// 			// If the last exit result is not 0,end the process.
	// 			break ;
	// 	if (node->u_node.and_or.op == CONNECT_OR_IF)
	// 		if (ctx->exit_code == 0)
	// 			// If the last exit result is 0,end the process.
	// 			break ;
	// 	node = node->next;
	// }
	// return (result);
}

/*
	Todo
	- First and last simple cmds requires separate process
	- Other simple cmds has the same process
	- also subshell may happens here.
*/
t_status	exec_pipeline(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	int			pipe_fd[2];
	int			prev_read_fd;

	while (node != NULL)
	{
		// node = node->next;
	}
}
