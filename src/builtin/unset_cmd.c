/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:41:53 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/15 16:21:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/parser.h"
#include "../../include/strutil.h"

static bool	is_identifier(char *name)
{
	return (*name != '\0' && name[str_name_len(name)] == '\0');
}

/*
	unset name...
	"unset" unset the args from env table, and update the ctx->exit_code.
	If no parameters, it does nothing.
	Every name is looked at on its own: a name that cannot be one is
	reported and the status becomes a failure, but the names around it
	are still unset. bash reaches the same result by way of its function
	name space, which this shell does not have. [plan 15.1]
*/
t_status	unset_cmd(t_word_list *args, t_ctx *ctx)
{
	t_bucket_contents	*item;
	t_status			status;

	status = ST_OK;
	while (args)
	{
		if (!is_identifier(args->wd->str))
		{
			print_error_name("unset", args->wd->str,
				"not a valid identifier");
			status = ST_FAILURE;
		}
		else
		{
			item = hash_remove(args->wd->str, ctx->env_table);
			if (item != NULL)
			{
				free(item->key);
				free(item->data.value);
				free(item);
			}
		}
		args = args->next;
	}
	return (status);
}
