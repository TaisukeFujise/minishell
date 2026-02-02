/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdio_backup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:10:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/02 23:20:33 by tafujise         ###   ########.fr       */
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
		return (ST_FATAL);
	saved->stdout = dup(STDOUT_FILENO);
	if (saved->stdout < 0)
		return (ST_FATAL);
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
*/
t_status	undo_stdio(t_redirect *redirects, t_savedfd saved)
{
	if (dup2(saved.stdin, STDIN_FILENO) < 0)
		return (ST_FATAL);
	if (dup2(saved.stdout, STDOUT_FILENO) < 0)
		return (ST_FATAL);
	return (ST_OK);
}
