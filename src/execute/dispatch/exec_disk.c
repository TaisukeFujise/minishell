/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_disk.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:49:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/13 15:58:43 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	execute disk command(external command), like ls.
	Fork regardless of whether pipe_in or pipe_out are not a NO_PIPE.
	(It means whether command is connected by pipe or not doesn't matter.)
	- fork
	- in child : exec_disk_command
	- in parent:
		-set ctx->already_forked flag
		- close_pipes(pipe_in, pipe_out)
		- register_pid(ctx, pid)
*/
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ST_FAILURE);
	else if (pid == 0)
	{
		close_fd_bitmap(ctx->bitmap);
		if (attach_pipe_to_stdio(pipe_in, pipe_out) != ST_OK)
			exit(EXIT_FAILURE);
		pipe_in = NO_PIPE;
		pipe_out = NO_PIPE;
		if (apply_redirects(cmd->redirects) != ST_OK)
			exit(EXIT_FAILURE);
		if (apply_assigns(ctx->tmp_table, cmd->assigns, TMP) != ST_OK)
			exit(EXIT_FAILURE);
		exit(disk_command(cmd->args, ctx));
	}
	else
	{
		ctx->already_forked = 1;
		close_pipes(pipe_in, pipe_out);
		return (register_pid(ctx, pid));
	}
}
