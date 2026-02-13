/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:34:14 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:04:14 by tafujise         ###   ########.fr       */
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
		if (pipe_in > 0)
			close(pipe_in);
	}
	if (pipe_out != NO_PIPE)
	{
		if (dup2(pipe_out, STDOUT_FILENO) < 0)
			return (ST_FAILURE);
		if (pipe_out > 0)
			close(pipe_out);
	}
	return (ST_OK);
}

void	close_pipes(int pipe_in, int pipe_out)
{
	if (pipe_in != NO_PIPE)
		close(pipe_in);
	if (pipe_out != NO_PIPE)
		close(pipe_out);
}
