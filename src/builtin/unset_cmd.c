/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:41:53 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/25 00:23:09 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/parser.h"

static void	print_invalid_identifier(char *name)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static bool	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0' || ft_isdigit(str[i]))
		return (false);
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

/*
	unset name...
	"unset" unset the args from env table, and update the ctx->exit_code.
	If no parameters, it does nothing.
*/
t_status	unset_cmd(t_word_list *args, t_ctx *ctx)
{
	t_word_list			*cursor;
	t_bucket_contents	*item;

	cursor = args;
	while (cursor)
	{
		if (!is_valid_identifier(cursor->wd->str))
			return (print_invalid_identifier(cursor->wd->str), ST_FAILURE);
		cursor = cursor->next;
	}
	while (args)
	{
		item = hash_remove(args->wd->str, ctx->env_table);
		if (item != NULL)
		{
			free(item->key);
			free(item->data.value);
			free(item);
		}
		args = args->next;
	}
	return (ST_OK);
}
