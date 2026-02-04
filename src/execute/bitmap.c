/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:52:59 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/28 00:00:28 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"

t_fd_bitmap *new_fd_bitmap(int size)
{
	t_fd_bitmap *new;

	new = ft_calloc(sizeof(t_fd_bitmap), 1);
	if (new == NULL)
		return (NULL);
	else
	{
		new->bitmap = ft_calloc(sizeof(char), size);
		if (new->bitmap == NULL)
			return (NULL);
	}
	return (new);
}

void	close_fd_bitmap(t_fd_bitmap *fd_bitmap)
{
	int	i;

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
	free(fd_bitmap->bitmap);
	free(fd_bitmap);
}
