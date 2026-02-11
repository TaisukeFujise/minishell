/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:41:53 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 12:54:23 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/parser.h"

/*
	unset name...
	"unset" unset the args from env table, and update the ctx->exit_code.
	If no parameters, it does nothing.
*/
t_status	unset_cmd(t_word_list *args, t_ctx *ctx)
{
	int					i;
	t_bucket_contents	*item;

	i = 0;
	if (args == NULL)
		return (ST_FAILURE); // unset: not enough arguments
	if (args->wd->len < 1)
		return (ST_FAILURE);
	while (args->wd->str[i])
	{
		if (!(ft_isalnum(args->wd->str[i]) || args->wd->str[i] == '_'))
			return (ST_FAILURE);
				// unset: (args->wd->str): invalid parameter name
		i++;
	}
	if (ft_isdigit(*(args->wd->str)))
		return (ST_FAILURE);
	while (args)
	{
		item = hash_remove(args->wd->str, ctx->env_table);
		free(item);
		item = NULL;
		args = args->next;
	}
}
