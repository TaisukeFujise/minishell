/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/15 22:23:30 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/parser.h"

void	exec_complete(t_node *node, t_exec *executor)
{
	while (node != NULL)
	{
		execute(node->child, executor);
		node = node->next;
	}
}

void	exec_andor(t_node *node, t_exec *executor)
{
	while (node != NULL)
	{
		execute(node->child, executor);
		if (node->u_node.and_or.op == CONNECT_AND_IF)
			if (executor->exit_code != 0)
				// If the last exit status is not 0,end the process.
				break ;
		if (node->u_node.and_or.op == CONNECT_OR_IF)
			if (executor->exit_code == 0)
				// If the last exit status is 0,end the process.
				break ;
		node = node->next;
	}
}
//
void	exec_pipe(t_node *node, t_exec *executor)
{
	int		pipe_fd[2];
	int		prev_read_fd;
	pid_t	*pid;

	while (node != NULL)
	{
		/*
			TODO
			- First and last simple cmds requires separate process
			- Other simple cmds has the same process
			- also subshell may happens here.
		*/
		node = node->next;
	}
}

void	exec_simple(t_node *node, t_exec *executor)
{
	/*
		TODO
		- Create tmp_env_table from "t_word_list *assigns"
		- Expand Environment Variable
		- Set Redirect from "t_redirect *redirects"
		- Execute cmd, which is "built-in" or "execve",
			using executor->input_fd and executor->output_fd.
	*/
}

void	exec_subshell(t_node *node, t_exec *executor)
{
	// TODO
	// subshell
}

// Only dispatch exec_hogehoge by node_kind. (No recursive call of execute here)
void	execute(t_node *node, t_exec *executor)
{
	if (node == NULL) // when user_input is empty, ast root node is NULL.
		return ;       // it's NULL guard for thath
	if (node->node_kind == NODE_COMPLETE)
		exec_complete(node, executor);
	else if (node->node_kind == NODE_ANDOR)
		exec_andor(node, executor);
	else if (node->node_kind == NODE_PIPE) // 中でnext
		exec_pipe(node, executor);
	else if (node->node_kind == NODE_SUBSHELL)
		exec_subshell(node, executor);
	else if (node->node_kind == NODE_SIMPLE)
		exec_simple(node, executor);
	else
		exit(1);
}
