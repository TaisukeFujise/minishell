/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:01 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 15:19:59 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_valid_str(const char *str)
{
	if (str == NULL)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str != '\0')
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	check_sign(const char **str)
{
	if (**str == '-')
	{
		(*str)++;
		return (-1);
	}
	return (1);
}

static int	str2num_strict(const char **str, int sign)
{
	long long	num;

	num = 0;
	if (**str == '0')
		return (0);
	while (**str >= '0' && **str <= '9')
	{
		num *= 10;
		num += **str - '0';
		if (sign == 1 && (num > INT_MAX))
			return (INT_MAX);
		if (sign == -1
			&& ((long long)sign * num < (long long)(INT_MIN)))
			return (INT_MIN);
		(*str)++;
	}
	return ((int)(num * sign));
}

int	ft_strtol(const char *str)
{
	int	ans;
	int	sign;

	if (!check_valid_str(str))
		return (0);
	sign = check_sign(&str);
	ans = str2num_strict(&str, sign);
	return (ans);
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
