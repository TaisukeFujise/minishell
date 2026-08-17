/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:34:14 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/19 16:10:26 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	Put source on target and hand target the open file it names.
	Nothing to move and nothing to close when they are the same fd:
	closing would drop the fd the caller asked to keep.
*/
t_status	move_fd(int source, int target)
{
	if (source == target)
		return (ST_OK);
	if (dup2(source, target) < 0)
		return (ST_FAILURE);
	close(source);
	return (ST_OK);
}

t_status	attach_pipe_to_stdio(int pipe_in, int pipe_out)
{
	if (pipe_in != NO_PIPE && move_fd(pipe_in, STDIN_FILENO) != ST_OK)
		return (ST_FAILURE);
	if (pipe_out != NO_PIPE && move_fd(pipe_out, STDOUT_FILENO) != ST_OK)
		return (ST_FAILURE);
	return (ST_OK);
}

/*
	enter_child sets up the process boundary of a forked command:
	connect the pipe endpoints to stdio and close the fds inherited from
	the pipelines around this command. What the command itself needs,
	its redirects, is applied by the caller.
	The child cannot run anything if the boundary fails, so it exits.
*/
void	enter_child(t_ctx *ctx, int pipe_in, int pipe_out)
{
	if (attach_pipe_to_stdio(pipe_in, pipe_out) != ST_OK)
		exit(EXIT_FAILURE);
	close_fd_bitmap(ctx->bitmap);
}

void	close_pipes(int pipe_in, int pipe_out)
{
	if (pipe_in != NO_PIPE)
		close(pipe_in);
	if (pipe_out != NO_PIPE)
		close(pipe_out);
}
