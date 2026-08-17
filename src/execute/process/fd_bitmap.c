/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_bitmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:52:59 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 20:16:32 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"

t_fd_bitmap	*new_fd_bitmap(int size)
{
	t_fd_bitmap	*new;

	new = ft_calloc(sizeof(t_fd_bitmap), 1);
	if (new == NULL)
		return (NULL);
	new->bitmap = ft_calloc(sizeof(char), size);
	if (new->bitmap == NULL)
		return (free(new), NULL);
	new->size = size;
	return (new);
}

/*
	A copy of src that can also hold fd, for the child of one pipeline
	stage. A copy, not the same bitmap: what a stage adds must not stay
	behind for the commands that follow it.
*/
t_fd_bitmap	*grow_fd_bitmap(t_fd_bitmap *src, int fd)
{
	t_fd_bitmap	*copy;
	int			size;

	size = src->size;
	if (fd >= size)
		size = fd + 8;
	copy = new_fd_bitmap(size);
	if (copy == NULL)
		return (NULL);
	ft_memcpy(copy->bitmap, src->bitmap, src->size);
	return (copy);
}

void	close_fd_bitmap(t_fd_bitmap *fd_bitmap)
{
	int	i;

	if (fd_bitmap == NULL || fd_bitmap->bitmap == NULL)
		return ;
	i = 0;
	while (i < fd_bitmap->size)
	{
		if (fd_bitmap->bitmap[i])
		{
			close(i);
			fd_bitmap->bitmap[i] = 0;
		}
		i++;
	}
}

void	dispose_fd_bitmap(t_fd_bitmap *fd_bitmap)
{
	if (fd_bitmap == NULL)
		return ;
	free(fd_bitmap->bitmap);
	free(fd_bitmap);
}
