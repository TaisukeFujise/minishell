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

static int	open_tmp_write_fd(char **filename);
static int	open_heredoc_fd(t_redirect *redirect);
static int	open_redirect_fd(t_redirect *redirect);

/*
	apply_redirects call redirect func depending on redirect->op.
	- apply_redir_great
	- apply_redir_less
	- apply_redir_dgreat
	- apply_redir_dless
*/
t_status	apply_redirects(t_redirect *redirects)
{
	t_status	status;
	int			fd;

	status = ST_OK;
	while (redirects)
	{
		fd = open_redirect_fd(redirects);
		if (fd < 0)
			return (ST_FAILURE);
		if (dup2(fd, redirects->io_number) < 0)
			return (close(fd), ST_FAILURE);
		close(fd);
		redirects = redirects->next;
	}
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

static int	open_heredoc_fd(t_redirect *redirect)
{
	char	*filename;
	int		write_fd;
	int		read_fd;

	filename = NULL;
	write_fd = open_tmp_write_fd(&filename);
	if (write_fd < 0)
		return (-1);
	if (write(write_fd, redirect->hd.raw_str.str, redirect->hd.raw_str.len) < 0)
		return (free(filename), close(write_fd), -1);
	close(write_fd);
	read_fd = open(filename, O_RDONLY);
	if (read_fd < 0)
		return (free(filename), -1);
	if (unlink(filename) < 0)
		return (free(filename), close(read_fd), -1);
	redirect->hd.content_fd = read_fd;
	return (free(filename), read_fd);
}

static int	open_tmp_write_fd(char **filename)
{
	int	fd;

	fd = -1;
	while (fd < 0)
	{
		free(*filename);
		*filename = create_tmp_filename();
		if (*filename == NULL)
			return (-1);
		fd = open(*filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd < 0 && errno != EEXIST)
			return (free(*filename), *filename = NULL, -1);
	}
	return (fd);
}
