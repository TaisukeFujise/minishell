/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:23:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/minishell.h"
#include "../../include/parser.h"

/*
	execute init fd_bitmap and wrap execute_internal.
	after the func call, dispose the fd_bitmap, and return t_status result.
	parse error passes NULL root ast.
*/
t_status	execute(t_node *root, t_ctx *ctx)
{
	t_status	result;
	t_stage		st;

	if (root == NULL)
		return (ST_OK);
	st = new_stage(NO_PIPE, NO_PIPE);
	st.close = new_fd_bitmap(FD_BITMAP_SIZE);
	if (st.close == NULL)
		return (ST_FATAL);
	result = execute_internal(root, ctx, st);
	dispose_fd_bitmap(st.close);
	return (result);
}

/*
	execute_internal call exec_*** by the type of node,
		and return t_status result.

	- exec_subshell
	- exec_simple
	- exec_connection

	Whoever starts a process arranges for it to be waited for: a pipeline
	stage goes into the process set of the pipeline, any other command is
	waited for by the code that forked it.
*/
t_status	execute_internal(t_node *node, t_ctx *ctx, t_stage st)
{
	if (node == NULL)
		return (ST_OK);
	if (node->node_kind == NODE_SUBSHELL)
		return (exec_subshell(node, ctx, st));
	if (node->node_kind == NODE_SIMPLE)
		return (exec_simple(node, ctx, st));
	if (node->node_kind == NODE_COMPLETE || node->node_kind == NODE_ANDOR
		|| node->node_kind == NODE_PIPE)
		return (exec_connection(node, ctx, st));
	return (ST_FATAL);
}

/*
	How many processes a pipeline can start, counted before the first
	fork so that the process set is allocated once. [review PROC-01]
*/
int	count_stages(t_node *node)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
		return (count_stages(node->left) + count_stages(node->right));
	return (1);
}
