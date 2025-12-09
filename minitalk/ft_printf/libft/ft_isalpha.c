/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:10 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:14:11 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (c == -1)
		return (0);
	if ((uc <= 'Z' && uc >= 'A') || (uc <= 'z' && uc >= 'a'))
		return (1);
	return (0);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_isalpha(int c)
// {
// 	char *pri = ft_isalpha(c) ? "alphabet" : "not alphabet";
// 	char *tpri = isalpha(c) ? "alphabet" : "not alphabet";
// 	printf("ft_isalpha : %c = %s", c, pri);
// 	printf("\n");
// 	printf("isalpha    : %c = %s", c, tpri);
// 	printf("\n");
// }

// int main(void)
// {
// 	printf("Testing ft_isalpha:\n");
// 	test_ft_isalpha('A');
// 	test_ft_isalpha('a');
// 	test_ft_isalpha('Z');
// 	test_ft_isalpha('z');
// 	test_ft_isalpha('0');
// 	test_ft_isalpha('/');
// 	test_ft_isalpha(-1);
// 	printf("\n");
// 	return 0;
// }