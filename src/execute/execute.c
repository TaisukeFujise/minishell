/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/18 22:33:02 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

/*
	全てのexec_*とexecute関数の返り値をステータスの構造体を返すように変更し
	返り値の確認をして正常なステータス以外であれば、そこでreturnする設計に変更
*/

t_status	exec_complete(t_node *node, t_exec *executor, t_ctx *ctx)
{
	t_status	status;

	while (node != NULL)
	{
		status = execute(node->child, executor, ctx);
		if (status == ST_FATAL)
			return (ST_FATAL);
		node = node->next;
	}
	return (status);
}

t_status	exec_andor(t_node *node, t_exec *executor, t_ctx *ctx)
{
	int	status;

	while (node != NULL)
	{
		status = execute(node->child, executor, ctx);
		if (status == ST_FATAL)
			return (ST_FATAL);
		if (node->u_node.and_or.op == CONNECT_AND_IF)
			if (ctx->exit_code != 0)
				// If the last exit status is not 0,end the process.
				break ;
		if (node->u_node.and_or.op == CONNECT_OR_IF)
			if (ctx->exit_code == 0)
				// If the last exit status is 0,end the process.
				break ;
		node = node->next;
	}
	return (status);
}
//
t_status	exec_pipe(t_node *node, t_exec *executor, t_ctx *ctx)
{
	t_status	status;
	int			pipe_fd[2];
	int			prev_read_fd;
	pid_t		*pids;

	pids = malloc(sizeof(pid_t) * count_simple_cmds(node));// nodeのポインタを進めないように内部でt_node *tmpに渡す
	if (pids == NULL)
		return (ST_FATAL);
	while (node != NULL)
	{
		/*
			Todo
			- First and last simple cmds requires separate process
			- Other simple cmds has the same process
			- also subshell may happens here.
		*/
		node = node->next;
	}
	// wait pids
	free(pids);
}

t_status	exec_simple(t_simple_cmd *cmd, t_exec *executor, t_ctx *ctx)
{
	/*
		Todo
		- Create tmp_env_table from "t_word_list *assigns"
		- Expand Environment Variable
		- Set Redirect from "t_redirect *redirects"
		- Execute cmd, which is "built-in" or "execve",
			using executor->input_fd and executor->output_fd.
	*/
	t_status	status;

	status = load_assigns_to_table(executor->tmp_table, cmd->assigns);
	if (status == ST_FATAL)
		return (status);
	status = expand_args(cmd, executor, ctx);
	if (status == ST_FATAL)
		return (status);
	status = apply_redirects(cmd, executor, ctx);
	if (status == ST_FATAL)
		return (status);
	return (execute_cmd(cmd, executor, ctx));
}

t_status	exec_subshell(t_node *node, t_exec *executor, t_ctx *ctx)
{
	// Todo
	// subshell
}

// Only dispatch exec_hogehoge by node_kind. (No recursive call of execute here)
t_status	execute(t_node *node, t_exec *executor, t_ctx *ctx)
{
	t_status	status;

	if (node == NULL) // when user_input is empty, ast root node is NULL.
		return (ST_FATAL); // it's NULL guard.
	if (node->node_kind == NODE_COMPLETE)
	{
		status = exec_complete(node, executor, ctx);
		if (status == ST_FATAL) {return (ST_FATAL);}
	} else if (node->node_kind == NODE_ANDOR)
	{
		status = exec_andor(node, executor, ctx);
		if (status == ST_FATAL) {return (ST_FATAL);}
	} else if (node->node_kind == NODE_PIPE) // 中でnext
	{
		status = exec_pipe(node, executor, ctx);
		if (status == ST_FATAL) {return (ST_FATAL);}
	} else if (node->node_kind == NODE_SUBSHELL)
	{
		status = exec_subshell(node, executor, ctx);
		if (status == ST_FATAL) {return (ST_FATAL);}
	} else if (node->node_kind == NODE_SIMPLE)
	{
		status = exec_simple(&(node->u_node.simple_command), executor, ctx);
		if (status == ST_FATAL) {return (ST_FATAL);}
	} else
		return (ST_FATAL);
}
