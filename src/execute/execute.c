/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/24 23:16:32 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

/*
	全てのexec_*とexecute関数の返り値をステータスの構造体を返すように変更し
	返り値の確認をして正常なステータス以外であれば、そこでreturnする設計に変更
*/
t_status	exec_complete(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{

	// while (node != NULL)
	// {
	// 	exec_result = execute(node->child, executor, ctx);
	// 	if (exec_result == ST_FATAL)
	// 		return (ST_FATAL);
	// 	node = node->next;
	// }
	// return (exec_result);
}

t_status	exec_andor(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{

	// while (node != NULL)
	// {
	// 	exec_result = execute(node->child, executor, ctx);
	// 	if (exec_result == ST_FATAL)
	// 		return (ST_FATAL);
	// 	if (node->u_node.and_or.op == CONNECT_AND_IF)
	// 		if (ctx->exit_code != 0)
	// 			// If the last exit exec_result is not 0,end the process.
	// 			break ;
	// 	if (node->u_node.and_or.op == CONNECT_OR_IF)
	// 		if (ctx->exit_code == 0)
	// 			// If the last exit exec_result is 0,end the process.
	// 			break ;
	// 	node = node->next;
	// }
	// return (exec_result);
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
	pid_t		*pids;

	pids = malloc(sizeof(pid_t) * count_simple_cmds(node));// nodeのポインタを進めないように内部でt_node *tmpに渡す
	if (pids == NULL)
		return (ST_FATAL);
	while (node != NULL)
	{
		// node = node->next;
	}
	// wait pids
	free(pids);
}

t_status	exec_simple_command(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	/*
		Todo
		- Create tmp_env_table from "t_word_list *assigns"
		- Expand Environment Variable
		- Set Redirect from "t_redirect *redirects"
		- Execute cmd, which is "built-in" or "execve",
			using executor->input_fd and executor->output_fd.
	*/
	t_status	exec_result;
	int			dofork;// Flag which decide whether fork or not.

	hash_flush(ctx->tmp_table, NULL); //cleanup the content of tmp table, not tmp_table itself, built by assignment word.
	if (load_assigns_to_table(ctx->tmp_table, node->u_node.simple_command.assigns)
		== ST_FATAL)
		return (ST_FATAL);
	if (expand_args(node->u_node.simple_command.args, ctx, ctx)
		== ST_FATAL)
		return (ST_FATAL);
	if (apply_redirects(node->u_node.simple_command.redirects, ctx, ctx)
		== ST_FATAL)
		return (ST_FATAL);
	dofork = pipe_in != NO_PIPE || pipe_out != NO_PIPE;
	/*
		- search built-in
		- execute built-in command
		- execute disk command(external command)
	*/
	// Let's make flag whether it's forked or not.
	if (dofork)
	{
		/*
			fork here
			- built-in: in child
			- external: in child(no fork in exec_disk_command)
		*/
	}
	else
	{
		/*
			no fork here
			- built-in: in parent
			- external: in child(fork in exec_disk_command)
		*/
	}
}

t_status	exec_subshell(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	// Todo
	// subshell
}

// Only dispatch exec_hogehoge by node_kind. (No recursive call of execute here)
t_status	execute(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	if (node == NULL) // when user_input is empty, ast root node is NULL.
		return (ST_FATAL); // it's NULL guard.
	if (node->node_kind == NODE_COMPLETE)
		return (exec_complete(node, ctx, pipe_in, pipe_out));
	else if (node->node_kind == NODE_ANDOR)
		return (exec_andor(node, ctx, pipe_in, pipe_out));
	else if (node->node_kind == NODE_PIPE) // 中でnext
		return (exec_pipe(node, ctx, pipe_in, pipe_out));
	else if (node->node_kind == NODE_SUBSHELL)
		return (exec_subshell(node, ctx, pipe_in, pipe_out));
	else if (node->node_kind == NODE_SIMPLE)
		return (exec_simple(&(node->u_node.simple_command), ctx, pipe_in, pipe_out));
	else
		return (ST_FATAL);
}
