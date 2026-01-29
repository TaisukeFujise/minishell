/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:09:15 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/29 18:19:32 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

t_status	load_assigns_to_table(t_hashtable *tmp_table, t_word_list *assigns)
{
	// hash_flush(ctx->tmp_table, NULL); //cleanup the content of tmp table, not tmp_table itself, built by assignment word.
	// hash_flushはload_assigns_to_tableでする
}

/*
	Todo
	- Create tmp_env_table from "t_word_list *assigns"
	- Expand Environment Variable
	- Set Redirect from "t_redirect *redirects"
	- Execute cmd, which is "built-in" or "execve",
		using executor->input_fd and executor->output_fd.
	- Update last_pid in ctx in order to waitpid.
*/
t_status	exec_simple(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	ctx->already_forked = 0; // initialize ctx->already_forked flag.
	// if (pipe_in != NO_PIPE || pipe_out != NO_PIPE)
	// {
	// 	if (make_child() == 0)
	// 	{
	// 		already_forked = 1;
	// 		close_fd_bitmap(ctx->bitmap);
	// 		do_piping(pipe_in, pipe_out);
	// 		pipe_in = pipe_out = NO_PIPE;
	// 	}
	// 	else
	// 	{
	// 		close(pipe_in);
	// 		close(pipe_out);
	// 		return (ST_OK);
	// 	}
	// }
	// expand args and assignment word!!!.
	if (expand_args(node->u_node.simple_command, ctx)
		== ST_FATAL)
		return (ST_FATAL);
	if (load_assigns_to_table(ctx->tmp_table, node->u_node.simple_command.assigns)
		== ST_FATAL)
		return (ST_FATAL);
	if (node->u_node.simple_command.args == 0)
		return (exec_null_command(node->u_node.simple_command.redirects,
				ctx, pipe_in, pipe_out));
	if (find_builtin(node->u_node.simple_command.args[0]))
		return (exec_builtin(node->u_node.simple_command,
				ctx, pipe_in, pipe_out));
	return (exec_disk_command(node->u_node.simple_command, ctx, pipe_in, pipe_out));
}
