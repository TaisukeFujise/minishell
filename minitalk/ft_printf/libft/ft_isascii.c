/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:12 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:14:13 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (uc < 128)
		return (1);
	return (0);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_isascii(int c)
// {
// 	char *pri = ft_isascii(c) ? "ascii" : "not ascii";
// 	char *tpri = isascii(c) ? "ascii" : "not ascii";
// 	printf("ft_isascii : %d = %s", c, pri);
// 	printf("\n");
// 	printf("isascii    : %d = %s", c, tpri);
// 	printf("\n");
// }

// int main(void)
// {
// 	printf("Testing ft_isascii:\n");
// 	test_ft_isascii(127);
// 	test_ft_isascii(128);
// 	test_ft_isascii(0);
// 	test_ft_isascii(-1);
// 	test_ft_isascii(-50);
// 	return (0);
// }
