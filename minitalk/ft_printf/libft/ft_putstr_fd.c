/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:26 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:27 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>

// void	test_ft_putstr_fd(char **strs)
// {
// 	int fd = open("ft_putstr_fd.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
// 	if (fd == -1) {
// 		perror("Error opening file");
// 		return ;
// 	}
// 	for (int i = 0; strs[i] != NULL; i++)
// 		ft_putstr_fd(strs[i], fd);
// 	close(fd);
// }

// int main(void)
// {
// 	char *strs[] = {"Hello ", "World!", NULL};
// 	test_ft_putstr_fd(strs);
// 	return 0;
// }
