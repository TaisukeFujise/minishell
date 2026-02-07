/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdio_backup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:10:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/06 19:11:14 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

/*
	save_stdio is called by null or builtin cmd in parent process.
	save STDIN_FILENO and STDOUT_FILENO to saved.stdin and saved.stdout
*/
t_status	save_stdio(t_savedfd *saved)
{
	saved->stdin = dup(STDIN_FILENO);
	if (saved->stdin < 0)
		return (ST_FAILURE);
	saved->stdout = dup(STDOUT_FILENO);
	if (saved->stdout < 0)
	{
		close(saved->stdin);
		return (ST_FAILURE);
	}
	return (ST_OK);
}

void	close_savedfd(t_savedfd saved)
{
	close(saved.stdin);
	close(saved.stdout);
}
/*
	restore_stdio is called by null or builtin cmd in parent process.
	undo STDIN_FILENO and STDOUT_FILENO by saved.stdin and saved.stdout
	This function's failure is ST_FATAL, because the main process stdio is never reversed if this func fails.
*/
t_status	undo_stdio(t_savedfd saved)
{
	if (dup2(saved.stdin, STDIN_FILENO) < 0)
		return (ST_FATAL);
	if (dup2(saved.stdout, STDOUT_FILENO) < 0)
		return (ST_FATAL);
	return (ST_OK);
}
