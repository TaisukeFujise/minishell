/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:16:03 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:16:04 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (c == -1)
		return (c);
	if (uc >= 'A' && uc <= 'Z')
		uc += 32;
	return (uc);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_tolower(int c)
// {
// 	char tol = ft_tolower(c);
// 	char ttol = tolower(c);
// 	printf("ft_tolower : %c = %c", c, tol);
// 	printf("\n");
// 	printf("tolower    : %c = %c", c, ttol);
// 	printf("\n");
// }

// int main(void)
// {
// 	test_ft_tolower('A');
// 	test_ft_tolower('Z');
// 	test_ft_tolower('a');
// 	test_ft_tolower('z');
// 	test_ft_tolower('0');
// 	test_ft_tolower('-');
// 	test_ft_tolower(127);
// 	return 0;
// }
