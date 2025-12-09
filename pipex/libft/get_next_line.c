/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 18:32:04 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/01 20:56:32 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*read_to_stash(int fd, char *stash);
static char	*extract_line(char *stash);
static char	*update_stash(char *stash);

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (stash == NULL)
		return (NULL);
	line = extract_line(stash);
	if (line == NULL)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = update_stash(stash);
	return (line);
}

static char	*read_to_stash(int fd, char *stash)
{
	char	*buffer;
	int		bytes;
	char	*new_stash;

	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (buffer == NULL)
		return (free(stash), NULL);
	bytes = 1;
	while (bytes != 0 && ft_strchr(buffer, '\n') == NULL)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(stash), free(buffer), NULL);
		buffer[bytes] = '\0';
		new_stash = ft_strjoin(stash, buffer);
		free(stash);
		stash = new_stash;
	}
	return (free(buffer), stash);
}

static char	*extract_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	if (stash == NULL || stash[i] == '\0')
		return (NULL);
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	if (line == NULL)
		return (NULL);
	ft_memcpy(line, stash, i);
	if (stash[i] == '\n')
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*update_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	if (stash == NULL)
		return (NULL);
	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	if (stash[i] == '\0')
		return (free(stash), NULL);
	new_stash = ft_calloc(ft_strlen(stash) - i + 1, sizeof(char));
	if (new_stash == NULL)
		return (free(stash), NULL);
	i++;
	j = 0;
	while (stash[i] != '\0')
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}
