/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:01 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 16:29:51 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	skip_space(const char **str)
{
	while (**str != '\0' && (**str == ' ' || (**str >= 9 && **str <= 13)))
		(*str)++;
}

static int	check_sign(const char **str)
{
	if (**str == '-')
	{
		(*str)++;
		return (-1);
	}
	else if (**str == '+')
	{
		(*str)++;
		return (1);
	}
	return (1);
}

static int	str2num(const char **str)
{
	int	num;

	num = 0;
	while (**str >= '0' && **str <= '9')
	{
		num *= 10;
		num += **str - '0';
		(*str)++;
	}
	return (num);
}

int	ft_atoi(const char *str)
{
	int	ans;
	int	sign;

	skip_space(&str);
	sign = check_sign(&str);
	ans = str2num(&str);
	return (sign * ans);
}

// #include <stdio.h>

// void	test_ft_atoi(char *str)
// {
// 	printf("ft_atoi: %d\n", ft_atoi(str));
// 	printf("atoi   : %d\n", atoi(str));
// }

// int main(void)
// {
// 	printf("Testing ft_atoi:\n");
// 	test_ft_atoi("   -1234567890   82793582 ");
// 	test_ft_atoi("42");
// 	test_ft_atoi("   +0abc");
// 	printf("\n");
// 	return 0;
// }
