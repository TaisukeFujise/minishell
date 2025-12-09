/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:34 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:35 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

// #include <stdio.h>
// #include <stdlib.h>

// void	add_i(unsigned int i, char *c)
// {
// 	*c += i;
// }

// void	test_ft_striteri(void)
// {
// 	char s[] = "abcdef";
// 	printf("s   = \"%s\"\n", s);
// 	ft_striteri(s, add_i);
// 	printf("s   = \"%s\"\n", s);
// }

// int main(void)
// {
// 	test_ft_striteri();
// 	return 0;
// }