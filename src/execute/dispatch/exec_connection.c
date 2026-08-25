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

t_status	exec_connection(t_node *node, t_ctx *ctx)
{
	if (node->node_kind == NODE_COMPLETE)
		return (exec_complete(node, ctx));
	if (node->node_kind == NODE_ANDOR)
		return (exec_andor(node, ctx));
	if (node->node_kind == NODE_PIPE)
		return (exec_pipeline(node, ctx));
	return (ST_FATAL);
}

t_status	exec_complete(t_node *node, t_ctx *ctx)
{
	t_status	result;

	result = execute_internal(node->left, ctx, EXEC_SHELL_PROCESS);
	if (result == ST_EXIT || result == ST_FATAL)
		return (result);
	if (node->right == NULL)
		return (result);
	return (execute_internal(node->right, ctx, EXEC_SHELL_PROCESS));
}

/*
	&& and || look at the status of the left hand command, which is the
	number it left in ctx->err.exit_code. A t_status only says whether
	the shell can go on.
*/
t_status	exec_andor(t_node *node, t_ctx *ctx)
{
	t_status	result;

	result = execute_internal(node->left, ctx, EXEC_SHELL_PROCESS);
	if (result == ST_EXIT || result == ST_FATAL)
		return (result);
	if (node->u_node.and_or.op == CONNECT_AND_IF && ctx->err.exit_code != 0)
		return (result);
	if (node->u_node.and_or.op == CONNECT_OR_IF && ctx->err.exit_code == 0)
		return (result);
	return (execute_internal(node->right, ctx, EXEC_SHELL_PROCESS));
}
