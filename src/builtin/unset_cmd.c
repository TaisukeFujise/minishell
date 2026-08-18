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

/*
	unset name...
	"unset" unset the args from env table, and update the ctx->exit_code.
	If no parameters, it does nothing.
	A word that cannot be a name is passed over in silence, and the
	status stays 0. bash does the same, by handing such a word to its
	function name space; POSIX does not say what should happen; and a
	word that cannot be a name never matches an entry anyway. dash
	reports an error instead, so this follows bash rather than a rule.
*/
t_status	unset_cmd(t_word_list *args, t_ctx *ctx)
{
	t_bucket_contents	*item;

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
