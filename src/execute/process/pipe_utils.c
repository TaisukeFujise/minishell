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
	A stage that holds nothing: no pipe, no fd to close in a child.
	Callers fill in what they own. One place knows the fields.
*/
t_stage	new_stage(int pipe_in, int pipe_out)
{
	t_stage	st;

	st.pipe_in = pipe_in;
	st.pipe_out = pipe_out;
	st.close = NULL;
	return (st);
}

/*
	Put source on target and hand target the open file it names.
	Nothing to close when they are the same fd: closing would drop it.
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

/*
	enter_child sets up the process boundary of a forked command:
	connect the pipe endpoints to stdio and close the fds inherited from
	the pipelines around this command. What the command itself needs,
	its redirects, is applied by the caller.
	The child cannot run anything if the boundary fails, so it exits.
*/
void	enter_child(t_stage st)
{
	if (st.pipe_in != NO_PIPE && move_fd(st.pipe_in, STDIN_FILENO) != ST_OK)
		exit(EXIT_FAILURE);
	if (st.pipe_out != NO_PIPE && move_fd(st.pipe_out, STDOUT_FILENO) != ST_OK)
		exit(EXIT_FAILURE);
	close_fd_bitmap(st.close);
}

void	close_pipes(t_stage st)
{
	if (st.pipe_in != NO_PIPE)
		close(st.pipe_in);
	if (st.pipe_out != NO_PIPE)
		close(st.pipe_out);
}
