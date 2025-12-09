/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:23 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 16:31:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int nbr)
{
	return ((nbr ^ (nbr >> 31)) - (nbr >> 31));
}

void	ft_putnbr_fd(int n, int fd)
{
	char	cur_digit;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = ft_abs(n);
	}
	cur_digit = n % 10 + '0';
	if (n / 10 > 0)
		ft_putnbr_fd(n / 10, fd);
	write(fd, &cur_digit, 1);
}

// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>

// void	test_ft_putnbr_fd(int nums[], int size)
// {
// 	int fd = open("ft_putnbr_fd.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
// 	if (fd == -1) {
// 		perror("Error opening file");
// 		return ;
// 	}
// 	for (int i = 0; i < size; i++)
// 	{
// 		ft_putnbr_fd(nums[i], fd);
// 		write(fd, "\n", 1);
// 	}
// 	close(fd);
// }

// int main(void)
// {
// 	int nums[4] = {-2147483648, 0, 12345, -67890};
// 	test_ft_putnbr_fd(nums, 4);
// 	return 0;
// }
