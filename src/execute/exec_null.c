/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_null.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:51:30 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/31 18:58:01 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

/*
	execute null command, meaning no args command.
	- fork if pipe_in or pipe_out is not a NO_PIPE. (It means command is connected by pipe)
	- fork
		- restore_original_signals()
		- fork: make_child()
		- do_piping
		- apply_redirects
	- no fork
		- apply_redirects
		- cleanup_redirects

	- apply_redirects
*/
t_status	exec_null_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;
	pid_t		pid;

	if (pipe_in != NO_PIPE || pipe_out != NO_PIPE)
	{
		pid = make_child(ctx);
		if (pid < 0)
			return (ST_FATAL);
		else if (pid == 0)
		{
			if (apply_assigns_to_vars(ctx->env_table, cmd->assigns) == ST_FATAL)
				return (ST_FATAL);
			close_fd_bitmap(ctx->bitmap);
			if (do_piping(ctx, pipe_in, pipe_out) == ST_FATAL)
				exit(EXIT_FAILURE);
			pipe_in = pipe_out = NO_PIPE;
			if (apply_redirects(cmd->redirects, ctx) != ST_OK)
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else
			return (close_pipes(pipe_in, pipe_out), ST_OK);
	}
	else
	{
		if (apply_assigns_to_vars(ctx->env_table, cmd->assigns) == ST_FATAL)
			return (ST_FATAL);
		if (apply_redirects(cmd->redirects, ctx) == ST_FATAL)
			return (ST_FATAL);
		return (cleanup_redirects(cmd->redirects, ctx));
	}
}

t_status	do_piping(t_ctx *ctx, int pipe_in, int pipe_out) 
{
	if (pipe_in != NO_PIPE)
	{
		if (dup2(pipe_in, STDIN_FILENO) < 0)
			return (ST_FATAL);
		if (pipe_in < 0)
			close(pipe_in);
	}
	if (pipe_out != NO_PIPE)
	{
		if (dup2(pipe_out, STDOUT_FILENO) < 0)
			return (ST_FATAL);
		if (pipe_out < 0)
			close(pipe_out);
	}
	return (ST_OK);
}

pid_t	make_child(t_ctx *ctx)
{
	/*
		Todo
		- set already_forked flag
		- fork()
		- stack pid on *pids
		- increment npid
	*/
	// pid_t	pid;

	// ctx->already_forked = true;
	// pid = fork();
	// if (pid < 0)
	// 	return 
}
