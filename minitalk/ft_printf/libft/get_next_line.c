/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:55:41 by fendo             #+#    #+#             */
/*   Updated: 2025/10/31 15:50:38 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <unistd.h>

char	*update_buffer(char **buffer, int i, char *cur_line)
{
	char	*new_buffer;

	if ((*buffer)[i] == '\n')
	{
		new_buffer
			= ft_strndup_gnl(*buffer + i + 1, ft_strlen_gnl(*buffer + i + 1));
		if (!new_buffer)
		{
			free(*buffer);
			*buffer = NULL;
			free(cur_line);
			return (NULL);
		}
	}
	else
		new_buffer = NULL;
	free(*buffer);
	*buffer = new_buffer;
	return (cur_line);
}

char	*get_line_and_update_buffer(char **buffer)
{
	char	*cur_line;
	int		i;

	i = 0;
	while ((*buffer)[i] != '\n' && (*buffer)[i] != '\0')
		i++;
	if ((*buffer)[i] == '\n')
		cur_line = ft_strndup_gnl(*buffer, i + 1);
	else
		cur_line = ft_strndup_gnl(*buffer, i);
	if (!cur_line)
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	return (update_buffer(buffer, i, cur_line));
}

int	read_file(int fd, char **buffer, char **buf, int i)
{
	int		read_byte;
	char	*tmp;

	read_byte = read(fd, *buf, BUFFER_SIZE);
	if (read_byte == -1)
		return (-1);
	else if (read_byte == 0)
		return (0);
	(*buf)[read_byte] = '\0';
	tmp = ft_strjoin_gnl(*buffer, *buf);
	if (!tmp)
	{
		*buffer = NULL;
		return (-1);
	}
	*buffer = tmp;
	if (ft_strchr_gnl(*buffer + i - 1, '\n'))
		return (0);
	i += read_byte;
	return (i);
}

char	*read_until_nl(int fd, char *buffer)
{
	int		i;
	char	*buf;

	if (!buffer)
		buffer = ft_calloc_gnl(1, sizeof(char));
	if (!buffer)
		return (NULL);
	buf = ft_calloc_gnl(BUFFER_SIZE + 1, sizeof(char));
	if (!buf)
	{
		free(buffer);
		return (NULL);
	}
	i = 1;
	while (i > 0)
	{
		i = read_file(fd, &buffer, &buf, i);
		if (i == -1 && buffer)
			buffer[0] = '\0';
	}
	free(buf);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*cur_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_until_nl(fd, buffer);
	if (!buffer)
		return (NULL);
	if (buffer[0] == '\0')
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	cur_line = get_line_and_update_buffer(&buffer);
	return (cur_line);
}
