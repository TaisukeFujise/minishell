/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:37:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/25 19:37:23 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"
#include "../../include/shell.h"

static int	_load_envp_to_table(t_hashtable *env_table, char **envp);
static int	_load_env_entry(t_hashtable *env_table, char *entry);
static char	*_extract_key_from_envp(char *entry);
static char	*_extract_value_from_envp(char *entry);

int	init_ctx(t_ctx *ctx, char **envp)
{
	ft_bzero(ctx, sizeof(t_ctx));
	ctx->interactive = (isatty(STDIN_FILENO) == 1
			&& isatty(STDERR_FILENO) == 1);
	ctx->env_table = hash_create(BUCKET_SIZE);
	if (ctx->env_table == NULL)
		return (FAILURE);
	ctx->tmp_table = hash_create(BUCKET_SIZE);
	if (ctx->tmp_table == NULL || _load_envp_to_table(ctx->env_table,
			envp) == FAILURE || init_shell_vars(ctx) == FAILURE)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
		hash_dispose(ctx->tmp_table);
		return (FAILURE);
	}
	return (SUCCESS);
}

/*
	An environment string is name=value, so a string without an "=" and
	one whose name is empty are not variables at all; bash walks past
	both rather than starting without them.
	[bash-5.3 variables.c initialize_shell_variables()]
*/
static int	_load_envp_to_table(t_hashtable *env_table, char **envp)
{
	if (envp == NULL)
		return (FAILURE);
	while (*envp != NULL)
	{
		if (**envp != '=' && ft_strchr(*envp, '=') != NULL)
		{
			if (_load_env_entry(env_table, *envp) == FAILURE)
				return (FAILURE);
		}
		envp++;
	}
	return (SUCCESS);
}

static int	_load_env_entry(t_hashtable *env_table, char *entry)
{
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	key = _extract_key_from_envp(entry);
	if (key == NULL)
		return (FAILURE);
	item = hash_insert(key, env_table);
	free(key);
	if (item == NULL)
		return (FAILURE);
	value = _extract_value_from_envp(entry);
	if (value == NULL)
		return (FAILURE);
	if (!hash_set_value(item, value))
		return (free(value), FAILURE);
	free(value);
	item->data.exported = true;
	return (SUCCESS);
}

static char	*_extract_key_from_envp(char *entry)
{
	int	i;

	i = 0;
	while (entry[i] != '=' && entry[i] != '\0')
		i++;
	if (entry[i] == '\0')
		return (NULL);
	return (ft_strndup(entry, i));
}

static char	*_extract_value_from_envp(char *entry)
{
	int	i;

	i = 0;
	while (entry[i] != '=' && entry[i] != '\0')
		i++;
	if (entry[i] == '\0')
		return (NULL);
	return (ft_strdup(entry + i + 1));
}
