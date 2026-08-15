/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tmpfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 09:09:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 18:48:24 by fujisetaisuke    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"

char	*create_tmp_filename(void)
{
	static unsigned int	i = 0;
	char				*num;
	char				*filename;

	if (i > INT_MAX)
		return (NULL);
	num = ft_itoa(i);
	if (num == NULL)
		return (NULL);
	filename = ft_strjoin("tmp_", num);
	free(num);
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
