/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:45:59 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/04 20:35:22 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

void		exec_builtin_in_pipe(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	builtin_cmd(t_word_list *args, t_ctx *ctx);

/*
	execute builtin command, like cd.
	Fork if pipe_in or pipe_out is not a NO_PIPE.
	(It means command is connected by pipe)
	- in pipe
		- fork
		- in child : exec_builtin_command_in_pipe
		- in parent:
			- set ctx->already_forked flag
			- close_pipes(pipe_in, pipe_out)
			- register_pid(ctx, pid)
	- single
		- exec_builtin_in_parent
*/
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	pid_t	pid;

	if (pipe_in != NO_PIPE || pipe_out != NO_PIPE)
	{
		pid = fork();
		if (pid < 0)
			return (ST_FAILURE);
		else if (pid == 0)
			exec_builtin_in_pipe(cmd, ctx, pipe_in, pipe_out);
		else
		{
			ctx->already_forked = 1;
			close_pipes(pipe_in, pipe_out);
			return (register_pid(ctx, pid));
		}
	}
	else
		return (exec_builtin_in_parent(cmd, ctx, pipe_in, pipe_out));
}

/*
	exec_builtin_in_pipe is called in pipe
	- close_fd_bitmap
	- apply_redirects
	- apply_assigns_to_tmp_env
	- builtin_cmd
*/
void	exec_builtin_in_pipe(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	/*
		Todo left
		- restore_signals ???
	*/
	close_fd_bitmap(ctx->bitmap);
	if (attach_pipe_to_stdio(pipe_in, pipe_out) == ST_FATAL)
		exit (EXIT_FAILURE);
	pipe_in = pipe_out = NO_PIPE;
	if (apply_redirects(cmd->redirects) == ST_FATAL)
		exit (EXIT_FAILURE);
	if (apply_assigns_to_tmp_env(ctx->tmp_table, cmd->assigns) == ST_FATAL)
		exit (EXIT_FAILURE);
	exit(builtin_cmd(cmd->args, ctx));
}

/*
	exec_builtin_in_parent
	- save_stdio
	- apply_redirects
	- apply_assigns_to_tmp_env
	- builtin_cmd
	- undo_stdio
*/
t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;
	t_savedfd	saved;

	if (save_stdio(&saved) != ST_OK)
		return (ST_FAILURE);
	if (apply_redirects(cmd->redirects) != ST_OK)
	{
		close_savedfd(saved);
		return (ST_FAILURE);
	}
	if (apply_assigns_to_tmp_env(ctx->tmp_table, cmd->assigns) == ST_FATAL)
	{
		close_savefd(saved);
		return (undo_stdio(saved));// It doesn't matter if this func fails or not.
	}
	if (builtin_cmd(cmd->args, ctx) != ST_OK)
	{
		close_savedfd(saved);
		return (undo_stdio(saved));//It doesn't matter fi this func fails or not.
	}
	result = undo_stdio(saved);
	close_savedfd(saved);
	return (result);
}

/*
	builtin_cmd dispatch builtin command
	by comparing args->wd->str[0] with some builtin cmd strings.
	- echo
	- cd
	- pwd
	- export
	- unset
	- env
	- exit
*/
t_status	builtin_cmd(t_word_list *args, t_ctx *ctx)
{
	/*
		Todo
		- builtin_command find builtin cmd and execute it.
	*/	
	if (args->wd->str == "cd")
		return (cd_cmd(args, ctx));
	else if (args->wd->str == "echo")
		return (echo_cmd(args, ctx));
	else if (args->wd->str == "env")
		return (env_cmd(args, ctx));
	else if (args->wd->str == "exit")
		return (exit_cmd(args, ctx));
	else if (args->wd->str == "export")
		return (export_cmd(args, ctx));
	else if (args->wd->str == "pwd")
		return (pwd_cmd(args, ctx));
	else if (args->wd->str == "unset")
		return (unset_cmd(args, ctx));
	else
		return (ST_FATAL);
}
