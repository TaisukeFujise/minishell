/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:16:05 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:16:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	unsigned char	uc;

	uc = (unsigned char) c;
	if (c == -1)
		return (c);
	if (uc >= 'a' && uc <= 'z')
		uc -= 32;
	return (uc);
}

// #include <stdio.h>
// #include <ctype.h>

// void	test_ft_toupper(int c)
// {
// 	char tol = ft_toupper(c);
// 	char ttol = toupper(c);
// 	printf("ft_toupper : %c = %c", c, tol);
// 	printf("\n");
// 	printf("toupper    : %c = %c", c, ttol);
// 	printf("\n");
// }

// int main(void)
// {
// 	test_ft_toupper('A');
// 	test_ft_toupper('Z');
// 	test_ft_toupper('a');
// 	test_ft_toupper('z');
// 	test_ft_toupper('0');
// 	test_ft_toupper('-');
// 	test_ft_toupper(127);
// 	return 0;
// }
