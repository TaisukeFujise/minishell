/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tmpfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 09:09:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 18:48:24 by fujisetaisuke    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"

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
	filename = ft_strjoin("tmp_", num);
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
		fd = open(*filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd < 0 && errno != EEXIST)
			return (free(*filename), *filename = NULL, -1);
	}
	return (fd);
}

/*
	A write to a regular file can stop short, so keep writing until the
	whole body is out. No progress is a failure.
*/
static bool	write_all(int fd, char *buf, size_t len)
{
	ssize_t	written;
	size_t	done;

	done = 0;
	while (done < len)
	{
		written = write(fd, buf + done, len - done);
		if (written <= 0)
			return (false);
		done += (size_t)written;
	}
	return (true);
}

/*
	Put the body of a heredoc in a temporary file and open it for reading.
	The name is unlinked once both ends are settled, so no error path
	leaves an entry behind. The caller owns the fd this returns; the AST
	keeps no fd of its own.
*/
int	open_heredoc_fd(t_redirect *redirect)
{
	char	*filename;
	int		fd;

	filename = NULL;
	fd = open_tmp_write_fd(&filename);
	if (fd < 0)
		return (-1);
	if (!write_all(fd, redirect->hd.raw_str.str, redirect->hd.raw_str.len))
		return (close(fd), unlink(filename), free(filename), -1);
	close(fd);
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	return (fd);
}

// int	main(void)
// {
// 	printf("%s\n", create_tmp_filename());
// 	printf("%s\n", create_tmp_filename());
// 	printf("%s\n", create_tmp_filename());
// 	printf("%s\n", create_tmp_filename());
// }
