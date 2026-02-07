/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:52:59 by tafujise          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/02/06 18:38:07 by tafujise         ###   ########.fr       */
=======
/*   Updated: 2026/02/05 14:39:17 by tafujise         ###   ########.fr       */
>>>>>>> 97f86b5e26d55c3d9117fb0e2643c6c2f80c40eb
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"

t_fd_bitmap *new_fd_bitmap(int size)
{
	t_fd_bitmap *new;

	new = ft_calloc(sizeof(t_fd_bitmap), 1);
	if (new == NULL)
		return (NULL);
	new->bitmap = ft_calloc(sizeof(char), size);
	if (new->bitmap == NULL)
		return (NULL);
	new->size = size;
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
