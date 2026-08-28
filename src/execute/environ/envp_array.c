/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 19:30:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 19:30:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"

char	*make_env_entry(char *key, char *value)
{
	char	*entry;
	char	*head;
	int		i;

	if (key == NULL || value == NULL)
		return (NULL);
	entry = malloc(sizeof(char) * (ft_strlen(key) + ft_strlen(value) + 2));
	if (entry == NULL)
		return (NULL);
	i = 0;
	head = entry;
	while (key[i])
	{
		*entry++ = key[i];
		i++;
	}
	*entry++ = '=';
	i = 0;
	while (value[i])
	{
		*entry++ = value[i];
		i++;
	}
	*entry = '\0';
	return (head);
}

/*
	Walk one table and append the entries the merged environment keeps.
	From env_table we skip the keys tmp_table overrides, they come from
	the tmp_table walk. Entries that are not exported are shell variables.
*/
static char	**walk_table(t_hashtable *table, t_hashtable *skip, char **envp)
{
	int					i;
	t_bucket_contents	*item;

	i = 0;
	while (table != NULL && i < table->bucket_size)
	{
		item = hash_items(i, table);
		while (item != NULL)
		{
			if (item->data.exported && item->data.value != NULL
				&& (skip == NULL || hash_search(item->key, skip) == NULL))
			{
				*envp = make_env_entry(item->key, item->data.value);
				if (*envp == NULL)
					return (NULL);
				envp++;
			}
			item = item->next;
		}
		i++;
	}
	return (envp);
}

/*
	Build the environment of the current command from the persistent table
	and the assignments of this command. tmp_table wins on the same key.
	The result is a NULL terminated array the caller owns. One slot is
	left free for set_underscore().
*/
char	**build_envp(t_hashtable *tmp_table, t_hashtable *env_table)
{
	char	**envp;
	char	**tail;
	int		count;

	count = 2;
	if (tmp_table != NULL)
		count += tmp_table->entry_count;
	if (env_table != NULL)
		count += env_table->entry_count;
	envp = ft_calloc(count, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	tail = walk_table(tmp_table, NULL, envp);
	if (tail != NULL)
		tail = walk_table(env_table, tmp_table, tail);
	if (tail == NULL)
		return (free_envp(envp), NULL);
	*tail = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

/*
	Set _ to the pathname the command is about to become, in the
	environment that command gets. Without this a command reads the value
	the shell itself was started with. [review D37-21]
	build_envp() leaves one free slot for this entry.
*/
void	set_underscore(char **envp, char *pathname)
{
	char	*entry;
	int		i;

	entry = make_env_entry("_", pathname);
	if (entry == NULL)
		return ;
	i = 0;
	while (envp[i] != NULL && (envp[i][0] != '_' || envp[i][1] != '='))
		i++;
	if (envp[i] == NULL)
		envp[i + 1] = NULL;
	else
		free(envp[i]);
	envp[i] = entry;
}

/*
	Whether the candidate is a file that could have been run at all.
	execve() answers EACCES for a directory as well as for a file that
	may not be run, and only the second is worth reporting: bash leaves
	anything that is not a regular file out of the search, so "cd .."
	as a command ends as not found and not as a refusal.
*/
