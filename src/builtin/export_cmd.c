/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:50 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 12:41:23 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/parser.h"

static int	_isvalid_idchars(char *str, int len);
int			validate_identifier(t_word *wd);

/*
	export name[=word]...
	"export" register the args to env table, and update the ctx->exit_code.
	If no parameters, it displays exported vars.
	(i) no parameters
	(ii) only name
	(iii) key=value
*/
t_status	export_cmd(t_word_list *args, t_ctx *ctx)
{
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	if (args == NULL)
		print_env(ctx->tmp_table, ctx->env_table);
	while (args)
	{
		if (validate_identifier(args->wd) == FAILURE)
			return (ST_FAILURE);
		// export: (args->wd->str): not a valid identifier
		if (args->wd->flag & W_ASSIGN)
		{
			key = extract_key(args->wd->str);
			value = extract_value(args->wd->str);
			item = hash_insert(key, ctx->env_table);
			if (item == NULL)
				return (ST_FATAL);
			if (item->data.value != NULL)
			{
				free(item->data.value);
				item->data.value = NULL;
			}
			item->data.value = value;
		}
		else
		{
			// If args->wd is already in variable table, make it exported.
			item = hash_search(args->wd->str, ctx->env_table);
			if (item == NULL)
				return (ST_OK);
			item->data.exported = true;
		}
		args = args->next;
	}
	return (ST_OK);
}

/*
	Rule of identifier in POSIX.
	(i) available character
		[A-Z], [a-z], [0-9], "_"
	(ii) rule
		- At initial charcter, number is not allowed.
		- Minimum length of name is a one character.
*/
int	validate_identifier(t_word *wd)
{
	int	id_len;

	if (wd->flag & W_ASSIGN)
		id_len = count_id(wd->str);
	else
		id_len = wd->len;
	if (id_len < 1)
		return (FAILURE);
	if (_isvalid_idchars(wd->str, id_len) == FAILURE)
		return (FAILURE);
	if (ft_isdigit(*(wd->str)))
		return (FAILURE);
	return (SUCCESS);
}

/*
	available character
		[A-Z], [a-z], [0-9], "_"
*/
static int	_isvalid_idchars(char *str, int len)
{
	int	i;

	i = 0;
	while (str[i] && i < len)
	{
		if (!(ft_isalnum(str[i]) || (str[i] == '_')))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
