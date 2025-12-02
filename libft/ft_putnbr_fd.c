/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:47:56 by tafujise          #+#    #+#             */
/*   Updated: 2025/10/18 09:09:50 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	long_nbr;
	char	digit;

	long_nbr = n;
	if (long_nbr < 0)
	{
		write(fd, "-", 1);
		long_nbr *= -1;
	}
	if (long_nbr >= 10)
		ft_putnbr_fd(long_nbr / 10, fd);
	digit = (long_nbr % 10) + '0';
	write(fd, &digit, 1);
}
