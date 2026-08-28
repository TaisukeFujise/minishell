/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/28 19:06:58 by fendo            ###   ########.fr       */
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
	return (execute_internal(root, ctx, EXEC_MAY_FORK));
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
	The numeric status of a command comes from the command: from the
	builtin that ran, or from waiting for the child. This settles the
	status of what happens around it instead: an expansion, a fork or a
	redirect the shell could not carry out is a plain failure, and an
	internal failure that chose no code must not read as success.
	[review D37-12, D37-17]
*/
t_status	set_exit_code(t_ctx *ctx, t_status status)
{
	if (status == ST_OK)
		ctx->err.exit_code = 0;
	else if (status == ST_FAILURE)
		ctx->err.exit_code = 1;
	else if (status == ST_FATAL && ctx->err.exit_code == 0)
		ctx->err.exit_code = 1;
	return (status);
}
