/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:43 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:44 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	n;

	n = 0;
	while (s[n] != '\0')
		n++;
	return (n);
}

// #include <stdio.h>

// void	test_ft_strlen(void)
// {
// 	printf("len = %d\n", ft_strlen("strlen"));
// 	printf("len = %d\n", ft_strlen(""));
// }

// int main(void){
// 	test_ft_strlen();
// 	return 0;
// }
