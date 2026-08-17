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
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

static int	open_redirect_fd(t_redirect *redirect);

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
	apply_redirects call redirect func depending on redirect->op.
	- apply_redir_great
	- apply_redir_less
	- apply_redir_dgreat
	- apply_redir_dless
	A shell process that keeps running asks for undoable, so that the
	redirects of one command do not outlive it. A process that only runs
	this command does not: it exits or execs.
*/
t_status	apply_redirects(t_redirect *redirects, bool undoable)
{
	t_redirect	*redir;
	int			floor;
	int			fd;

	floor = backup_floor(redirects);
	redir = redirects;
	while (redir)
	{
		if (undoable)
		{
			redir->saved = dup_above(redir->io_number, floor);
			if (redir->saved < 0)
				redir->saved = FD_WAS_CLOSED;
		}
		fd = open_redirect_fd(redir);
		if (fd < 0)
			return (ST_FAILURE);
		if (move_fd(fd, redir->io_number) != ST_OK)
			return (close(fd), ST_FAILURE);
		redir = redir->next;
	}
	return (ST_OK);
}

static t_status	undo_one(t_redirect *redir)
{
	int	saved;

	saved = redir->saved;
	redir->saved = 0;
	if (saved == FD_WAS_CLOSED)
		return (close(redir->io_number), ST_OK);
	if (saved <= 0)
		return (ST_OK);
	if (dup2(saved, redir->io_number) < 0)
		return (close(saved), ST_FATAL);
	close(saved);
	return (ST_OK);
}

/*
	Put back what this command replaced, last redirect first, so that
	several redirects of one io number unwind to the state it started in.
*/
t_status	undo_redirects(t_redirect *redirects)
{
	t_status	status;

	if (redirects == NULL)
		return (ST_OK);
	status = undo_redirects(redirects->next);
	if (undo_one(redirects) != ST_OK)
		return (ST_FATAL);
	return (status);
}

static int	open_redirect_fd(t_redirect *redirect)
{
	if (redirect->op == REDIR_GREATER)
		return (open(redirect->target.str, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (redirect->op == REDIR_LESS)
		return (open(redirect->target.str, O_RDONLY, 0644));
	else if (redirect->op == REDIR_DGREATER)
		return (open(redirect->target.str, O_WRONLY | O_CREAT | O_APPEND,
				0644));
	else if (redirect->op == REDIR_DLESS)
		return (open_heredoc_fd(redirect));
	else
		return (-1);
}
