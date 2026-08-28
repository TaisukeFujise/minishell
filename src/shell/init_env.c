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

#include <sys/stat.h>

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
	The directory named by an inherited PWD, or nothing when it names
	something else. bash trusts PWD only this far, so a shell started
	with PWD=/etc from /tmp still answers /tmp. Two stat calls and no
	canonicalisation: outside posix mode bash keeps the string it was
	given, "/tmp/../tmp" included.
*/
static bool	pwd_is_current(char *value)
{
	struct stat	named;
	struct stat	here;

	if (value == NULL || *value != '/')
		return (false);
	if (stat(value, &named) != 0 || stat(".", &here) != 0)
		return (false);
	return (named.st_dev == here.st_dev && named.st_ino == here.st_ino);
}

/*
	PWD is where the shell is. bash writes it at startup when the
	environment brought none, or brought one that is not this
	directory, so that $PWD answers even in a shell started with an
	empty environment. OLDPWD is left alone: bash has none of its own
	until the first cd.
	[bash-5.3 variables.c set_pwd()]
*/
static int	init_pwd(t_ctx *ctx)
{
	t_bucket_contents	*item;
	char				*cwd;

	item = hash_search("PWD", ctx->env_table);
	if (item != NULL && pwd_is_current(item->data.value))
	{
		ctx->cwd = path_canon(ft_strdup(item->data.value));
		if (ctx->cwd == NULL)
			return (FAILURE);
		return (SUCCESS);
	}
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (SUCCESS);
	item = hash_insert("PWD", ctx->env_table);
	if (item == NULL)
		return (free(cwd), FAILURE);
	item->data.exported = true;
	if (!hash_set_value(item, cwd))
		return (free(cwd), FAILURE);
	ctx->cwd = cwd;
	return (SUCCESS);
}

/*
	OLDPWD is there from the start as a name with no value, which is
	what bash leaves behind: env passes over it, export lists it, and
	the first cd has somewhere already marked for export to write. A
	shell whose environment brought no OLDPWD would otherwise keep the
	one cd makes to itself. [bash-5.3 variables.c set_pwd()]
*/
int	init_shell_vars(t_ctx *ctx)
{
	t_bucket_contents	*item;

	if (bump_shell_level(ctx->env_table) == FAILURE)
		return (FAILURE);
	item = hash_insert("OLDPWD", ctx->env_table);
	if (item == NULL)
		return (FAILURE);
	item->data.exported = true;
	return (init_pwd(ctx));
}
