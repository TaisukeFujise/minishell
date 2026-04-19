/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:33:34 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/24 23:07:54 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

t_status	_update_oldpwd(t_hashtable *tmp_table, t_hashtable *env_table);
t_status	update_pwd(t_hashtable *tmp_table, t_hashtable *env_table,
				char *path);

/*
	cd [directory]
	"cd" change current working directory, and update the ctx->exit_code.
	If no parameters, change the current working directory to $HOME.
	- 1. no directory operand with no HOME environment
	- 2. no directory operand with HOME environment
	- 3. directory operand begins with a <slash> character
	- 4. the first component of the directory operand is dot or dot-dot
	<reference>
	[ https://pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html ]
*/
t_status	cd_cmd(t_word_list *args, t_ctx *ctx)
{
	t_bucket_contents	*home;
	char				*path;

	(void)ctx;
	if (args == NULL)
	{
		home = hash_search("HOME", ctx->tmp_table);
		if (home == NULL)
		{
			home = hash_search("HOME", ctx->env_table);
			if (home == NULL)
				return (ST_FAILURE); // minishell: cd: HOME not set
		}
		if (chdir(home->data.value) < 0)
			return (ST_FATAL); // perror can express
		return (update_pwd(ctx->tmp_table, ctx->env_table, home->data.value));
	}
	if (count_args(args) > 2)
		return (ST_FAILURE); // minishell: cd: too many arguments.
	if (chdir(args->wd->str) < 0)
		return (ST_FATAL);
	// cd: no such file or directory: <directory name> or cd: permission denied: <directory name>
	path = getcwd(NULL, 0);
	if (path == NULL)
		return (ST_FATAL);
	if (update_pwd(ctx->tmp_table, ctx->env_table, path) != ST_OK)
		return (free(path), ST_FATAL);
	return (ST_OK);
}

t_status	_update_oldpwd(t_hashtable *tmp_table, t_hashtable *env_table)
{
	t_bucket_contents	*pwd;
	char				*oldpwd_key;
	t_bucket_contents	*oldpwd;

	pwd = hash_search("PWD", tmp_table);
	if (pwd == NULL)
	{
		pwd = hash_search("PWD", env_table);
		if (pwd == NULL)
			return (ST_OK);
	}
	oldpwd_key = ft_strdup("OLDPWD");
	if (oldpwd_key == NULL)
		return (ST_FATAL);
	oldpwd = hash_insert(oldpwd_key, env_table);
	if (oldpwd == NULL)
		return (free(oldpwd_key), ST_FATAL);
	if (oldpwd->data.value != NULL)
	{
		free(oldpwd->data.value);
		oldpwd->data.value = NULL;
	}
	oldpwd->data.value = ft_strdup(pwd->data.value);
	if (oldpwd->data.value == NULL)
		return (free(oldpwd_key), ST_FATAL);
	return (ST_OK);
}

t_status	update_pwd(t_hashtable *tmp_table, t_hashtable *env_table,
		char *path)
{
	t_bucket_contents	*pwd;
	char				*pwd_key;
	char				*oldpwd_key;

	if (_update_oldpwd(tmp_table, env_table) != ST_OK)
		return (ST_FATAL);
	pwd_key = ft_strdup("PWD");
	if (pwd_key == NULL)
		return (ST_FATAL);
	pwd = hash_insert(pwd_key, env_table);
	if (pwd == NULL)
		return (free(pwd_key), ST_FATAL);
	if (pwd->data.value != NULL)
	{
		free(pwd->data.value);
		pwd->data.value = NULL;
	}
	pwd->data.value = path;
	if (pwd->data.value == NULL)
		return (free(pwd_key), ST_FATAL);
	return (ST_OK);
}
