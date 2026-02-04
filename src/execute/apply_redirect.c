/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:31:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/04 08:42:17 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

t_status	apply_redir_greater(t_redirect *redirect);
t_status	apply_redir_less(t_redirect *redirect);
t_status	apply_redir_dgreater(t_redirect *redirect);
t_status	apply_redir_dless(t_redirect *redirect);

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

	while (redirects)
	{
		if (redirects->op == REDIR_GREATER)
			status = apply_redir_greater(redirects);
		else if (redirects->op == REDIR_LESS)
			status = apply_redir_less(redirects);
		else if (redirects->op == REDIR_DGREATER)
			status = apply_redir_dgreater(redirects);
		else if (redirects->op == REDIR_DLESS)
			status = apply_redir_dless(redirects);
		else
			status = ST_FATAL;
		if (status != ST_OK)
			return (status);
		redirects = redirects->next;
	}
	return (status);
}

/*
	apply_redir_great redirect output like " 3> file".
	- io_number "1" means " > file"
*/
t_status	apply_redir_greater(t_redirect *redirect)
{
	int	fd;

	fd = open(redirect->target.str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (ST_FAILURE);
	if (dup2(fd, redirect->io_number) < 0)
	{
		close(fd);
		return (ST_FAILURE);
	}
	close(fd);
	return (ST_OK);
}

/*
	apply_redir_less redirect input like " 4< file".
	- io_number "0" means " < file"
*/
t_status	apply_redir_less(t_redirect *redirect)
{
	int	fd;

	fd = open(redirect->target.str, O_RDONLY);
	if (fd < 0)
		return (ST_FAILURE);
	if (dup2(fd, redirect->io_number) < 0)
	{
		close(fd);
		return (ST_FAILURE);
	}
	close(fd);
	return (ST_OK);
}

/*
	apply_redir_dgreat redirect output in append mode like "4>> file"
	- io_number "1" means " >> file"
*/
t_status	apply_redir_dgreater(t_redirect *redirect)
{
	int	fd;

	fd = open(redirect->target.str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (ST_FAILURE);
	if (dup2(fd, redirect->io_number) < 0)
	{
		close(fd);
		return (ST_FAILURE);
	}
	close(fd);
	return (ST_OK);
}

/*
	apply_redir_dless redirect the input read by until a line containing the delimiter, like "4<< EOF"
	- io_number "0" means "<< file"
*/
t_status	apply_redir_dless(t_redirect *redirect)
{
	int		fd;
	char	*filename;
	int		len;

	fd = -1;
	while (fd < 0)
	{
		// filename = create_tmp_filename();
		filename = "tmp";
		if (filename == NULL)
			return (ST_FAILURE);
		fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0644);
		if (fd < 0 && errno != EEXIST)
			return (ST_FAILURE);
	}
	redirect->hd.content_fd = fd;
	if (unlink(filename) < 0)
		return (close(fd), ST_FAILURE);
	len = ft_strlen(redirect->hd.raw_str.str);
	if (write(fd, redirect->hd.raw_str.str, len) < 0)
		return (close(fd), ST_FAILURE);
	if (dup2(fd, redirect->io_number) < 0)
		return (close(fd), ST_FAILURE);
	close(fd);
	return (ST_OK);
}
