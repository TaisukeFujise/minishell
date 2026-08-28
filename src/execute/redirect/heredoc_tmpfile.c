/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tmpfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 09:09:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/27 20:34:08 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"

/*
	The file goes under /tmp, not the working directory: a shell must be
	able to read a here-document from a directory it cannot write to, and
	the name is not the user's to collide with. getpid() is not a function
	this project may call, so uniqueness comes from the counter and the
	O_EXCL open below, which retries when a name is already taken.
*/
char	*create_tmp_filename(void)
{
	static unsigned int	i = 0;
	char				*num;
	char				*filename;

	if (i > INT_MAX)
		return (NULL);
	num = ft_itoa(i);
	if (num == NULL)
		return (NULL);
	filename = ft_strjoin("/tmp/.minishell_hd_", num);
	free(num);
	if (filename == NULL)
		return (NULL);
	i++;
	return (filename);
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
		fd = open(*filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd < 0 && errno != EEXIST)
			return (free(*filename), *filename = NULL, -1);
	}
	return (fd);
}

/*
	The read fd belongs to the caller: the AST keeps no fd of its own.
	The name is removed on every path, so a failure leaves nothing behind.
	The file itself lives until the caller closes the fd.
*/
int	open_heredoc_fd(t_redirect *redirect)
{
	char	*filename;
	int		write_fd;
	int		read_fd;

	filename = NULL;
	write_fd = open_tmp_write_fd(&filename);
	if (write_fd < 0)
		return (-1);
	read_fd = -1;
	if (write_all(write_fd, redirect->hd.raw_str.str, redirect->hd.raw_str.len))
		read_fd = open(filename, O_RDONLY);
	close(write_fd);
	unlink(filename);
	free(filename);
	return (read_fd);
}
