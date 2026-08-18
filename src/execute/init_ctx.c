/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:37:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:23:47 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/hashmap.h"

char		*_extract_key_from_envp(char *entry);
char		*_extract_value_from_envp(char *entry);
static int	_load_envp_to_table(t_hashtable *env_table, char **envp);
static int	bump_shell_level(t_hashtable *env_table);

int	init_ctx(t_ctx *ctx, char **envp)
{
	ft_bzero(ctx, sizeof(t_ctx));
	ctx->env_table = hash_create(BUCKET_SIZE);
	if (ctx->env_table == NULL)
		return (FAILURE);
	ctx->tmp_table = hash_create(BUCKET_SIZE);
	if (ctx->tmp_table == NULL || _load_envp_to_table(ctx->env_table,
			envp) == FAILURE || bump_shell_level(ctx->env_table) == FAILURE)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
		hash_dispose(ctx->tmp_table);
		return (FAILURE);
	}
	return (SUCCESS);
}

// envp is reliable value, so we ignore the entry if the *envp doesn't have "="
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
