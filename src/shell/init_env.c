/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:37:10 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:37:11 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"
#include "../../include/shell.h"

/*
	SHLVL counts how deep this shell is. bash adds one to the value it
	was given, reads anything that is not a number as zero, and starts
	over at 1 above a thousand.
	[bash-5.3 variables.c adjust_shell_level()]
*/
static int	bump_shell_level(t_hashtable *env_table)
{
	t_bucket_contents	*item;
	char				*scan;
	char				*text;
	long				level;

	item = hash_insert("SHLVL", env_table);
	if (item == NULL)
		return (FAILURE);
	level = 0;
	scan = item->data.value;
	while (scan != NULL && ft_isdigit(*scan))
		scan++;
	if (scan != NULL && scan != item->data.value && *scan == '\0')
		level = ft_atol(item->data.value);
	level++;
	if (level >= 1000)
		level = 1;
	text = ft_itoa((int)level);
	if (text == NULL)
		return (FAILURE);
	item->data.exported = true;
	if (!hash_set_value(item, text))
		return (free(text), FAILURE);
	return (free(text), SUCCESS);
}

/*
	PWD is where the shell is. bash writes it at startup when the
	environment brought none, so that $PWD answers even in a shell
	started with an empty environment. OLDPWD is left alone: bash has
	none of its own until the first cd.
*/
static int	init_pwd(t_hashtable *env_table)
{
	t_bucket_contents	*item;
	char				*cwd;

	item = hash_search("PWD", env_table);
	if (item != NULL && item->data.value != NULL && *item->data.value != '\0')
		return (SUCCESS);
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (SUCCESS);
	item = hash_insert("PWD", env_table);
	if (item == NULL)
		return (free(cwd), FAILURE);
	item->data.exported = true;
	if (!hash_set_value(item, cwd))
		return (free(cwd), FAILURE);
	return (free(cwd), SUCCESS);
}

int	init_shell_vars(t_hashtable *env_table)
{
	if (bump_shell_level(env_table) == FAILURE)
		return (FAILURE);
	return (init_pwd(env_table));
}
