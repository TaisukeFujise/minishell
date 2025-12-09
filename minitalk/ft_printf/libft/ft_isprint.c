/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:16 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:14:17 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (c == -1)
		return (0);
	if (uc < 32 || uc > 126)
		return (0);
	return (1);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_isprint(int c)
// {
// 	char *pri = ft_isprint(c) ? "printable" : "not printable";
// 	char *tpri = isprint(c) ? "printable" : "not pirntable";
// 	printf("ft_isprint : %c = %s", c, pri);
// 	printf("\n");
// 	printf("isprint    : %c = %s", c, tpri);
// 	printf("\n");
// }

// int main(void)
// {
// 	test_ft_isprint(' ');
// 	test_ft_isprint('A');
// 	test_ft_isprint('1');
// 	test_ft_isprint(127);
// 	return 0;
// }
