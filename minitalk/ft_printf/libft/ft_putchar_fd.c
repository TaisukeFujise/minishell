/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:19 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:20 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>

// void	test_ft_putchar_fd(char *c)
// {
// 	int fd = open("ft_putchar_fd.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
// 	if (fd == -1) {
// 		perror("Error opening file");
// 		return ;
// 	}
// 	for (int i = 0; c[i] != '\0'; i++)
// 	{
// 		ft_putchar_fd(c[i], fd);
// 		ft_putchar_fd('\n', fd);
// 	}
// 	close(fd);
// }

// int main(void)
// {
// 	test_ft_putchar_fd("ABC");
// 	return 0;
// }