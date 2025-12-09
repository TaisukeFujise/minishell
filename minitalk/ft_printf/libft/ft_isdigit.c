/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:14 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:14:15 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (c == -1)
		return (0);
	if (uc >= '0' && uc <= '9')
		return (1);
	return (0);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_isdigit(int c)
// {
// 	char *pri = ft_isdigit(c) ? "digit" : "not digit";
// 	char *tpri = isdigit(c) ? "digit" : "not digit";
// 	printf("ft_isdigit : %c = %s", c, pri);
// 	printf("\n");
// 	printf("isdigit    : %c = %s", c, tpri);
// 	printf("\n");
// }

// int main(void)
// {
// 	printf("Testing ft_isdigit:\n");
// 	test_ft_isdigit('A');
// 	test_ft_isdigit('1');
// 	test_ft_isdigit('0');
// 	test_ft_isdigit(-1);
// 	return 0;
// }
