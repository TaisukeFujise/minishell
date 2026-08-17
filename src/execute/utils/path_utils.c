/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 02:39:44 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 01:15:32 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/parser.h"

char	*extract_path_value(t_hashtable *tmp_table, t_hashtable *env_table)
{
	return (env_lookup(tmp_table, env_table, "PATH"));
}

char	*extract_path_entry(char *path_value)
{
	int		i;
	char	*dir;

	i = 0;
	while (path_value[i])
	{
		if (path_value[i] == ':')
			break ;
		i++;
	}
	if (i == 0)
		return (ft_strdup("./"));
	dir = ft_strndup(path_value, i);
	if (dir == NULL)
		return (NULL);
	if (dir[ft_strlen(dir) - 1] == '/')
		return (dir);
	return (ft_strjoin(dir, "/"));
}
