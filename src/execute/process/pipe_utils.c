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

t_status	attach_pipe_to_stdio(int pipe_in, int pipe_out)
{
	if (pipe_in != NO_PIPE)
	{
		if (dup2(pipe_in, STDIN_FILENO) < 0)
			return (ST_FAILURE);
		close(pipe_in);
	}
	if (pipe_out != NO_PIPE)
	{
		if (dup2(pipe_out, STDOUT_FILENO) < 0)
			return (ST_FAILURE);
		close(pipe_out);
	}
	return (ST_OK);
}

/*
	enter_child is the process boundary of a forked command.
	Connect the pipe endpoints to stdio, close the fds inherited from
	the pipelines around this command, then apply the redirects.
	The child cannot run the command if a step fails, so it exits.
*/
void	enter_child(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	if (attach_pipe_to_stdio(pipe_in, pipe_out) != ST_OK)
		exit(EXIT_FAILURE);
	close_fd_bitmap(ctx->bitmap);
	if (apply_redirects(cmd->redirects) != ST_OK)
		exit(EXIT_FAILURE);
}

void	close_pipes(int pipe_in, int pipe_out)
{
	if (pipe_in != NO_PIPE)
		close(pipe_in);
	if (pipe_out != NO_PIPE)
		close(pipe_out);
}
