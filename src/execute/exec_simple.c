/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:15 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/28 00:11:07 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

t_status	exec_simple(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	/*
		Todo
		- Create tmp_env_table from "t_word_list *assigns"
		- Expand Environment Variable
		- Set Redirect from "t_redirect *redirects"
		- Execute cmd, which is "built-in" or "execve",
			using executor->input_fd and executor->output_fd.
		- Update last_pid in ctx in order to waitpid.
	*/
	t_status	result;
	int			dofork;// Flag which decide whether fork or not.

	hash_flush(ctx->tmp_table, NULL); //cleanup the content of tmp table, not tmp_table itself, built by assignment word.
	if (load_assigns_to_table(ctx->tmp_table, node->u_node.simple_command.assigns)
		== ST_FATAL)
		return (ST_FATAL);
	if (expand_args(node->u_node.simple_command.args, ctx, ctx)
		== ST_FATAL)
		return (ST_FATAL);
	// redirectはexecute_builtin, execute_disk_commandで処理される
	// if (apply_redirects(node->u_node.simple_command.redirects, ctx, ctx)
	// 	== ST_FATAL)
	// 	return (ST_FATAL);
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
