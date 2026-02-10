/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:33:34 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/10 14:08:48 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/builtin.h"

t_status	update_pwd(t_ctx *ctx, char *path);
int			count_args(t_word_list *args);

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
	(void)ctx;
	t_bucket_contents	*home;

	if (args == NULL)
	{
		home = hash_search("HOME", ctx->env_table);
		if (home == NULL)
			return (ST_FAILURE);// minishell: cd: HOME not set
		if (chdir(home->data.value) < 0)
			return (ST_FATAL);
		return (update_pwd(ctx, home->data.value));
	}
	if (count_args(args) > 2)
		return (ST_FAILURE);// minishell: cd: too many arguments.
}

t_status	update_pwd(t_ctx *ctx, char *path)
{
	t_bucket_contents	*pwd;
	t_bucket_contents	*oldpwd;
	char				*key_pwd;
	char				*key_oldpwd;

	pwd = hash_search("PWD", ctx->env_table);
	if (pwd == NULL)
	{	
		key_pwd = ft_strdup("PWD");
		if (key_pwd == NULL)
			return (ST_FATAL);
		pwd = hash_insert(key_pwd, ctx->env_table);
		if (pwd == NULL)
		{
			free(key_pwd);
			key_pwd = NULL;
			return (ST_FATAL);
		}
		if (pwd->data.value != NULL)
		{
			free(pwd->data.value);
			pwd->data.value = NULL;
		}
	}
	pwd->data.value = path;
	oldpwd = hash_search("OLDPWD", ctx->env_table);
	if (oldpwd == NULL)
	{
		key_oldpwd = ft_strdup("OLDPWD");
		if (key_oldpwd == NULL)
			return (ST_FATAL);
		oldpwd = hash_insert(key_oldpwd, ctx->env_table);
		if (oldpwd == NULL)
			return (ST_FATAL);
		oldpwd = hash_insert(key_oldpwd, ctx->env_table);
		if (oldpwd == NULL)
		{
			free(key_oldpwd);
			key_oldpwd = NULL;
			return (ST_FATAL);
		}
		if (oldpwd->data.value != NULL)
		{
			free(oldpwd->data.value);
			oldpwd->data.value = NULL;
		}
	}
	oldpwd->data.value = path;
	return (ST_OK);
}

int	count_args(t_word_list *args)
{
	int	count;

	count = 0;
	if (args == NULL)
		return (count);
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}
