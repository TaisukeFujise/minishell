/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:37:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/31 19:21:04 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/hashmap.h"

int			_preprocess_item(t_bucket_contents *item, char *key);
char		*_extract_key_from_envp(char *entry);
char		*_extract_value_from_envp(char *entry);
static int	_load_envp_to_table(t_hashtable *env_table, char **envp);

int	init_ctx(t_ctx *ctx, char **envp)
{
	ft_bzero(ctx, sizeof(t_ctx));

	/* init env_table by envp */
	ctx->env_table = hash_create(BUCKET_SIZE);
	if (ctx->env_table == NULL)
		return (FAILURE);
	if (_load_envp_to_table(ctx->env_table, envp) == FAILURE)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
		return (FAILURE);
	}
	/* init fd_bitmap */
	ctx->bitmap = new_fd_bitmap(FD_BITMAP_SIZE);
	if (ctx->bitmap == NULL)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
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
		if (preprocess_item(item, key) == FAILURE)
			return (FAILURE);
		value = _extract_value_from_envp(*envp);
		if (value == NULL)
			return (FAILURE);
		item->data.value = value;
		item->data.exported = true;
		envp++;
	}
	return (SUCCESS);
}

int	_preprocess_item(t_bucket_contents *item, char *key)
{
	if (item == NULL)
	{
		free(key);
		key = NULL;
		return (FAILURE);
	}
	if (item->data.value != NULL)
	{
		free(item->data.value);
		item->data.value = NULL;
	}
	return(SUCCESS);
}

char	*_extract_key_from_envp(char *entry)
{
	int		i;

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
	return (ft_strdup(entry + i));
}
