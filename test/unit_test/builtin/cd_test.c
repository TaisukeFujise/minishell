/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 14:46:20 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 14:46:23 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"

static int	g_ng;

static void	set_env_var(t_hashtable *table, char *key, char *value)
{
	t_bucket_contents	*entry;

	entry = hash_insert(ft_strdup(key), table);
	entry->data.value = ft_strdup(value);
	entry->data.exported = true;
}

static void	remove_env_var(t_hashtable *table, char *key)
{
	t_bucket_contents	*item;

	item = hash_remove(key, table);
	if (item == NULL)
		return ;
	free(item->key);
	free(item->data.value);
	free(item);
}

int	test_cd(char **envp)
{
	t_ctx		ctx;
	t_word_list	*args;
	char		origin[1024];
	char		cwd[1024];
	char		real_tmp[1024];

	g_ng = 0;
	getcwd(origin, sizeof(origin));
	realpath("/tmp", real_tmp);
	if (setup_ctx(&ctx, envp))
		return (1);
	set_env_var(ctx.env_table, "HOME", origin);
	CHECK(cd_cmd(NULL, &ctx) == ST_OK && getcwd(cwd, sizeof(cwd)) != NULL
		&& ft_strcmp(cwd, origin) == 0, "cd: no args with HOME");
	teardown_ctx(&ctx);
	if (setup_ctx(&ctx, envp))
		return (1);
	remove_env_var(ctx.env_table, "HOME");
	CHECK(cd_cmd(NULL, &ctx) == ST_FAILURE, "cd: no args without HOME");
	teardown_ctx(&ctx);
	if (setup_ctx(&ctx, envp))
		return (1);
	args = make_args((char *[]){"/tmp", NULL});
	CHECK(cd_cmd(args, &ctx) == ST_OK && getcwd(cwd, sizeof(cwd)) != NULL
		&& ft_strcmp(cwd, real_tmp) == 0, "cd: valid path");
	free_args(args);
	chdir(origin);
	teardown_ctx(&ctx);
	if (setup_ctx(&ctx, envp))
		return (1);
	args = make_args((char *[]){"/no_such_dir_xyz", NULL});
	CHECK(cd_cmd(args, &ctx) == ST_FATAL, "cd: invalid path");
	free_args(args);
	teardown_ctx(&ctx);
	if (setup_ctx(&ctx, envp))
		return (1);
	set_env_var(ctx.env_table, "PWD", origin);
	args = make_args((char *[]){"/tmp", NULL});
	CHECK(cd_cmd(args, &ctx) == ST_OK && hash_search("OLDPWD",
			ctx.env_table) != NULL && ft_strcmp(hash_search("OLDPWD",
				ctx.env_table)->data.value, origin) == 0, "cd: OLDPWD updated");
	free_args(args);
	chdir(origin);
	teardown_ctx(&ctx);
	return (g_ng);
}
