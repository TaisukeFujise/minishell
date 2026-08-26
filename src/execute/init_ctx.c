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

#include "../../include/execute.h"
#include "../../include/hashmap.h"

char		*_extract_key_from_envp(char *entry);
char		*_extract_value_from_envp(char *entry);
static int	_load_envp_to_table(t_hashtable *env_table, char **envp);

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
			envp) == FAILURE || init_shell_vars(ctx->env_table) == FAILURE)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
		hash_dispose(ctx->tmp_table);
		return (FAILURE);
	}
	return (SUCCESS);
}

// envp is reliable value, so we ignore the entry if the *envp doesn't have "="
static int	_load_envp_to_table(t_hashtable *env_table, char **envp)
{
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	if (envp == NULL)
		return (FAILURE);
	while (*envp != NULL)
	{
		key = _extract_key_from_envp(*envp);
		if (key == NULL)
			return (FAILURE);
		item = hash_insert(key, env_table);
		free(key);
		if (item == NULL)
			return (FAILURE);
		value = _extract_value_from_envp(*envp);
		if (value == NULL)
			return (FAILURE);
		if (!hash_set_value(item, value))
			return (free(value), FAILURE);
		free(value);
		item->data.exported = true;
		envp++;
	}
	return (SUCCESS);
}

char	*_extract_key_from_envp(char *entry)
{
	int	i;

	i = 0;
	while (entry[i] != '=' && entry[i] != '\0')
		i++;
	if (entry[i] == '\0')
		return (NULL);
	return (ft_strndup(entry, i));
}

char	*_extract_value_from_envp(char *entry)
{
	int	i;

	i = 0;
	while (entry[i] != '=' && entry[i] != '\0')
		i++;
	if (entry[i] == '\0')
		return (NULL);
	return (ft_strdup(entry + i + 1));
}
