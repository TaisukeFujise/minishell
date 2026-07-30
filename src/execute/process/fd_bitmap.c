/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_bitmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:52:59 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/19 16:12:26 by tafujise         ###   ########.fr       */
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
