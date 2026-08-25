/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:31:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 21:50:52 by fujisetaisuke    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include <string.h>
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

static int	open_redirect_fd(t_redirect *redirect);

/*
	Copy fd to a number at or above floor. dup() hands out the lowest free
	fd, which a later redirect of the same command would overwrite, so the
	low copies are held until a high one comes out. Returns -1 if fd is
	not open. bash moves backups the same way with fcntl(F_DUPFD).
*/
static int	dup_above(int fd, int floor)
{
	int	low;
	int	high;

	low = dup(fd);
	if (low < 0 || low >= floor)
		return (low);
	high = dup_above(fd, floor);
	close(low);
	return (high);
}

/*
	The backup of an io number must sit above every io number this command
	redirects, or one of them overwrites it.
*/
static int	backup_floor(t_redirect *redirects)
{
	int	floor;

	floor = 0;
	while (redirects)
	{
		if (redirects->io_number >= floor)
			floor = redirects->io_number + 1;
		redirects = redirects->next;
	}
	return (floor);
}

/*
	Apply the redirects of one command, in the order they were written.
	See t_redir_mode for what mode decides.
*/
t_status	apply_redirects(t_redirect *redirects, t_redir_mode mode)
{
	int	floor;
	int	fd;

	floor = 0;
	if (mode == REDIR_RESTORE)
		floor = backup_floor(redirects);
	while (redirects)
	{
		if (mode == REDIR_RESTORE)
		{
			redirects->saved = dup_above(redirects->io_number, floor);
			if (redirects->saved < 0)
				redirects->saved = FD_WAS_CLOSED;
		}
		fd = open_redirect_fd(redirects);
		if (fd < 0)
			return (ST_FAILURE);
		if (move_fd(fd, redirects->io_number) != ST_OK)
			return (close(fd), ST_FAILURE);
		redirects = redirects->next;
	}
	return (ST_OK);
}

/*
	Put back what this command replaced, last redirect first, so that
	several redirects of one io number unwind to the state it started in.
*/
t_status	undo_redirects(t_redirect *redirects)
{
	t_status	status;
	int			saved;

	if (redirects == NULL)
		return (ST_OK);
	status = undo_redirects(redirects->next);
	saved = redirects->saved;
	redirects->saved = 0;
	if (saved == FD_WAS_CLOSED)
		close(redirects->io_number);
	else if (saved > 0 && move_fd(saved, redirects->io_number) != ST_OK)
		return (close(saved), ST_FATAL);
	return (status);
}

/*
	The file a redirect names, or below zero with the reason already
	reported. A heredoc reports whatever went wrong with its own file.
*/
static int	open_redirect_fd(t_redirect *redirect)
{
	int	fd;

	if (redirect->op == REDIR_DLESS)
		return (open_heredoc_fd(redirect));
	if (redirect->op == REDIR_GREATER)
		fd = open(redirect->target.str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirect->op == REDIR_LESS)
		fd = open(redirect->target.str, O_RDONLY, 0644);
	else if (redirect->op == REDIR_DGREATER)
		fd = open(redirect->target.str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd < 0)
		print_error(redirect->target.str, strerror(errno));
	return (fd);
}
