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
	execute wraps execute_internal and returns a t_status result.
	parse error passes NULL root ast.
*/
t_status	execute(t_node *root, t_ctx *ctx)
{
	if (root == NULL)
		return (ST_OK);
	return (execute_internal(root, ctx, EXEC_SHELL_PROCESS));
}

/*
	execute_internal call exec_*** by the type of node,
		and return t_status result.

	- exec_subshell
	- exec_simple
	- exec_connection

	mode says which process this node is evaluated on: the shell itself,
	or a process that exists only for this node. See t_exec_mode.
*/
t_status	execute_internal(t_node *node, t_ctx *ctx, t_exec_mode mode)
{
	if (node == NULL)
		return (ST_OK);
	if (node->node_kind == NODE_SUBSHELL)
		return (exec_subshell(node, ctx, mode));
	if (node->node_kind == NODE_SIMPLE)
		return (exec_simple(node, ctx, mode));
	if (node->node_kind == NODE_COMPLETE || node->node_kind == NODE_ANDOR
		|| node->node_kind == NODE_PIPE)
		return (exec_connection(node, ctx));
	return (ST_FATAL);
}

/*
	How many processes a pipeline starts, counted before the first fork
	so that the process set is allocated once. [review PROC-01]
*/
int	count_stages(t_node *node)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
		return (count_stages(node->left) + count_stages(node->right));
	return (1);
}

/*
	The stages of a pipeline in the order they are written. The parse
	tree nests them to the left; the coordinator wants a flat list, the
	way dash keeps them in the node itself.
*/
t_node	**collect_stages(t_node *node, t_node **out)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
	{
		out = collect_stages(node->left, out);
		return (collect_stages(node->right, out));
	}
	*out = node;
	return (out + 1);
}
