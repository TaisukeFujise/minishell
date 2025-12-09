/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:18 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 16:30:23 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int				digits;
	char			*ans;
	int				i;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	digits = count_digits(n);
	ans = (char *)malloc(sizeof(char) * (digits + 1));
	i = digits - 1;
	if (n == 0)
		ans[0] = '0';
	else if (n < 0)
	{
		ans[0] = '-';
		n = -n;
	}
	while (n > 0)
	{
		ans[i--] = n % 10 + '0';
		n /= 10;
	}
	ans[digits] = '\0';
	return (ans);
}

// #include <stdio.h>

// void	test_ft_itoa(int n)
// {
// 	char *s = ft_itoa(n);
// 	printf("n   = %d\n", n);
// 	printf("str = %s\n", s);
// 	free(s);
// }

// int main(void)
// {
// 	test_ft_itoa(-2147483648);
// 	test_ft_itoa(0);
// 	test_ft_itoa(1234567890);
// 	return 0;
// }
