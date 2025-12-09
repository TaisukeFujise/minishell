/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:57 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 17:10:25 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_bool_set(char const *set, unsigned char bool_set[])
{
	size_t	i;

	i = 0;
	while (i < 256)
	{
		bool_set[i] = 0;
		i++;
	}
	i = 0;
	while (set[i] != '\0')
	{
		bool_set[(unsigned char)(set[i])] = 1;
		i++;
	}
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char			*str;
	unsigned char	bool_set[256];
	size_t			start;
	size_t			i;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		set = "";
	fill_bool_set(set, bool_set);
	i = 0;
	while (s1[i] != '\0' && bool_set[(unsigned char)(s1[i])] == 1)
		i++;
	start = i;
	while (s1[i] != '\0')
		i++;
	while (i > start && bool_set[(unsigned char)(s1[i - 1])] == 1)
		i--;
	str = (char *)malloc(i - start + 1);
	if (!str)
		return (NULL);
	str = ft_memcpy(str, s1 + start, i - start);
	str[i - start] = '\0';
	return (str);
}

// #include <stdio.h>
// #include <stdlib.h>

// static void	test_case(const char *s1, const char *set)
// {
// 	char *res = ft_strtrim(s1, set);

// 	printf("s1       : \"%s\"\n", s1 ? s1 : "(null)");
// 	printf("set      : \"%s\"\n", set ? set : "(null)");
// 	printf("=> result: \"%s\"\n", res ? res : "(null)");
// 	printf("----------------------------------------\n");

// 	free(res);
// }

// int	main(void)
// {
// 	printf("===== ft_strtrim TEST START =====\n\n");

// 	test_case("  hello  ", " ");
// 	test_case("xxaxxbxx", "x");
// 	test_case("ppabcppp", "p");
// 	test_case("aaa42Tokyoaaa", "a");

// 	test_case("", " ");
// 	test_case("abc", "");
// 	test_case("     ", " ");
// 	test_case("no_trim", "xyz");

// 	test_case("\t\n  spaced\t\n", " \n\t");
// 	test_case("$$money$$", "$");

// 	test_case(NULL, "a");
// 	test_case("abc", NULL);
// 	test_case(NULL, NULL);

// 	printf("===== ft_strtrim TEST END =====\n");
// 	return (0);
// }
