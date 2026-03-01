/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_null.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:51:30 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:07:39 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

void		exec_null_command_in_pipe(t_simple_cmd *cmd, t_ctx *ctx,
				int pipe_in, int pipe_out);
t_status	exec_null_command_in_parent(t_simple_cmd *cmd, t_ctx *ctx);

/*
	execute null command, meaning no args command.
	Fork if pipe_in or pipe_out is not a NO_PIPE.
	(It means command is connected by pipe)
	- in pipe
		- fork
		- in child : exec_null_command_in_pipe
		- in parent:
			- set ctx->already_forked flag
			- close_pipes(pipe_in, pipe_out)
			- register_pid(ctx, pid)
	- single
		- exec_null_command_in_parent
*/
t_status	exec_null_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	pid_t	pid;

	if (pipe_in != NO_PIPE || pipe_out != NO_PIPE)
	// command beside pipe in child process
	{
		pid = fork();
		if (pid < 0)
			return (ST_FAILURE);
		else if (pid == 0)
			exec_null_command_in_pipe(cmd, ctx, pipe_in, pipe_out);
		else
		{
			ctx->already_forked = 1;
			close_pipes(pipe_in, pipe_out);
			return (register_pid(ctx, pid));
		}
		return (ST_OK);
	}
	else // single command in parent process
		return (exec_null_command_in_parent(cmd, ctx));
}

/*
	exec_null_command_in_pipe is called in pipe.
	- close_fd_bitmp
	- attach_pipe_to_stdio
	- apply_redirects
	- apply_assigns_to_vars
*/
void	exec_null_command_in_pipe(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	/*
		Todo left
		- restore_signals ????
	*/
	close_fd_bitmap(ctx->bitmap);
	if (attach_pipe_to_stdio(pipe_in, pipe_out) != ST_OK)
		exit(EXIT_FAILURE);
	pipe_in = NO_PIPE;
	pipe_out = NO_PIPE;
	if (apply_redirects(cmd->redirects) != ST_OK)
		exit(EXIT_FAILURE);
	if (apply_assigns(ctx->env_table, cmd->assigns, VARS) != ST_OK)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

/*
	exec_null_command_in_parent is called in parent
	- save_stdio
	- apply_redirects
	- apply_assigns_to_vars
	- undo_stdio
*/
t_status	exec_null_command_in_parent(t_simple_cmd *cmd, t_ctx *ctx)
{
	t_status	result;
	t_savedfd	saved;

	if (save_stdio(&saved) != ST_OK)
		return (ST_FAILURE);
	if (apply_redirects(cmd->redirects) != ST_OK)
	{
		result = undo_stdio(saved);
		close_savedfd(saved);
		return (result);
	}
	if (apply_assigns(ctx->env_table, cmd->assigns, VARS) != ST_OK)
	{
		result = undo_stdio(saved);
		close_savedfd(saved);
		return (result);
	}
	result = undo_stdio(saved);
	close_savedfd(saved);
	return (result);
}
