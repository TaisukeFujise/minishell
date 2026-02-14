/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 02:39:44 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/15 02:39:46 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parser.h"

char	*extract_path_value(t_hashtable *tmp_table, t_hashtable *env_table)
{
	t_bucket_contents	*item;

	if (env_table == 0 || env_table->entry_count == 0)
		return (NULL);
	item = hash_search("PATH", tmp_table);
	if (item != NULL)
		return (item->data.value);
	item = hash_search("PATH", env_table);
	if (item != NULL)
		return (NULL);
	return (item->data.value);
}

char	*extract_path_entry(char *path_value)
{
	int		i;
	char	*dir;

	while (path_value[i])
	{
		if (path_value[i] == ":")
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
