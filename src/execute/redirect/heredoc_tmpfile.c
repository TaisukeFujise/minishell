/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_filename.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 09:09:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:03:33 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"

char	*create_tmp_filename(void)
{
	static int	i = 0;
	char		*filename;

	if (i > INT_MAX)
		return (NULL);
	filename = ft_itoa(i);
	if (filename == NULL)
		return (NULL);
	filename = ft_strjoin("tmp_", filename);
	if (filename == NULL)
		return (NULL);
	i++;
	return (filename);
}

// int	main(void)
// {
// 	printf("%s\n", create_tmp_filename());
// 	printf("%s\n", create_tmp_filename());
// 	printf("%s\n", create_tmp_filename());
// 	printf("%s\n", create_tmp_filename());
// }
