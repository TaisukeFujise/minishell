/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
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

void	close_fd(int fd)
{
	if (fd != NO_PIPE)
		close(fd);
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
