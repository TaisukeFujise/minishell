/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:26:51 by tafujise          #+#    #+#             */
/*   Updated: 2025/10/17 22:18:14 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_digit(long n)
{
	unsigned int	count;

	count = 0;
	if (n < 0)
	{
		count++;
		n *= -1;
	}
	if (n == 0)
		count++;
	while (n > 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

static char	*rev_num(char *str)
{
	size_t	i;
	size_t	j;
	char	tmp;

	i = 0;
	if (*str == '-')
		i = 1;
	while (i < ft_strlen(str))
	{
		j = i + 1;
		while (j < ft_strlen(str))
		{
			tmp = str[i];
			str[i] = str[j];
			str[j] = tmp;
			j++;
		}
		i++;
	}
	return (str);
}

static void	sign_process(long *num, char **str)
{
	if (*num < 0)
	{
		**str = '-';
		(*str)++;
		*num *= -1;
	}
}

char	*ft_itoa(int n)
{
	char			*res;
	long			long_n;
	unsigned int	digit_len;

	long_n = n;
	digit_len = count_digit(long_n);
	res = malloc(sizeof(char) * (digit_len + 1));
	if (!res)
		return (NULL);
	sign_process(&long_n, &res);
	if (long_n == 0)
	{
		*res = '0';
		res++;
	}
	while (long_n > 0)
	{
		*res = long_n % 10 + '0';
		res++;
		long_n = long_n / 10;
	}
	*res = '\0';
	return (rev_num(res - digit_len));
}
