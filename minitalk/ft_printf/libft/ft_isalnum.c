/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:09 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:14:10 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (c == -1)
		return (0);
	if ((uc <= 'Z' && uc >= 'A')
		|| (uc <= 'z' && uc >= 'a')
		|| (uc <= '9' && uc >= '0'))
		return (1);
	return (0);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_isalnum(int c)
// {
// 	char *pri = ft_isalnum(c) ? "alnum" : "not alnum";
// 	char *tpri = isalnum(c) ? "alnum" : "not alnum";
// 	printf("ft_isalnum : %c = %s", c, pri);
// 	printf("\n");
// 	printf("isalnum    : %c = %s", c, tpri);
// 	printf("\n");
// }

// int	main(void)
// {
// 	test_ft_isalnum('A');
// 	test_ft_isalnum('a');
// 	test_ft_isalnum('0');
// 	test_ft_isalnum('9');
// 	test_ft_isalnum('/');
// 	test_ft_isalnum(-1);
// 	printf("\n");
// }