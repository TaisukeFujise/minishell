/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:05:39 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:40:35 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	size_t			bytes;
	unsigned char	*arr;

	if (nmemb == 0 || size == 0)
	{
		return (malloc(0));
	}
	if (size != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	i = 0;
	bytes = nmemb * size;
	arr = malloc(bytes);
	if (!arr)
		return (NULL);
	while (i < bytes)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stddef.h>

// void    test_ft_calloc(size_t count, size_t size)
// {
//     int flag = 0;
//     int *arr = (int *)ft_calloc(count, size);
//     int *orig = (int *)calloc(count, size);
//     if (arr == NULL)
//     {
//         printf("ft_calloc failed to allocate memory.\n");
//         free(arr); 
//         free(orig);
//         flag = 1;
//     }
//     if (orig == NULL)
//     {
//         printf("calloc failed to allocate memory.\n");
//         free(arr);
//         free(orig);
//         flag = 1;
//     }

//     if (flag)
//         return;

//     printf("Allocated %zu integers.\n", count);
//     printf("Memory contents after ft_calloc:\n");

//     for (size_t i = 0; i < count; i++)
//     {
//         printf("ft_calloc: [%zu] = %d\n", i, arr[i]);
//         printf("calloc   : [%zu] = %d\n", i, orig[i]);
//     }

//     for (size_t i = 0; i < count; i++)
//     {
//         arr[i] = (int)(i);
//         orig[i] = (int)(i);
//     }

//     printf("\nAfter writing values:\n");
//     for (size_t i = 0; i < count; i++)
//     {
//         printf("ft_calloc: [%zu] = %d\n", i, arr[i]);
//         printf("calloc   : [%zu] = %d\n", i, orig[i]);
//     }

//     free(arr);
//     free(orig);
// }

// int main(void)
// {
//     printf("Testing ft_calloc:\n");
//     test_ft_calloc(10, sizeof(int));
//     test_ft_calloc(0, sizeof(int));
//     test_ft_calloc(100000000, 10000000);
//     test_ft_calloc(5, 0);
//     printf("\n");
//     return 0;
// }
