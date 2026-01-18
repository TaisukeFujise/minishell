/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/18 21:29:32 by tafujise         ###   ########.fr       */
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
	while (node != NULL)
	{
		execute(node->child, executor, ctx);
		node = node->next;
	}
}

t_status	exec_andor(t_node *node, t_exec *executor, t_ctx *ctx)
{
	while (node != NULL)
	{
		execute(node->child, executor, ctx);
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
}
//
t_status	exec_pipe(t_node *node, t_exec *executor, t_ctx *ctx)
{
	int		pipe_fd[2];
	int		prev_read_fd;
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count_simple_cmds(node));// nodeのポインタを進めないように内部でt_node *tmpに渡す
	if (pids == NULL)
		return ;
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
	// free(pids)
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
	if (load_assigns_to_table(executor->tmp_table, cmd->assigns) == FAILURE)
		return ;
	if (expand(cmd, executor, ctx) == FAILURE)
		return ;
}

t_status	exec_subshell(t_node *node, t_exec *executor, t_ctx *ctx)
{
	// Todo
	// subshell
}

// Only dispatch exec_hogehoge by node_kind. (No recursive call of execute here)
t_status	execute(t_node *node, t_exec *executor, t_ctx *ctx)
{
	if (node == NULL) // when user_input is empty, ast root node is NULL.
		return ; // it's NULL guard.
	if (node->node_kind == NODE_COMPLETE)
		exec_complete(node, executor, ctx);
	else if (node->node_kind == NODE_ANDOR)
		exec_andor(node, executor, ctx);
	else if (node->node_kind == NODE_PIPE) // 中でnext
		exec_pipe(node, executor, ctx);
	else if (node->node_kind == NODE_SUBSHELL)
		exec_subshell(node, executor, ctx);
	else if (node->node_kind == NODE_SIMPLE)
		exec_simple(&(node->u_node.simple_command), executor, ctx);
	else
		exit(1);
}
