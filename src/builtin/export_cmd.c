/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:50 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 17:14:08 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/strutil.h"

#define EXPORT_CTRL "\033\a\b\t\n\v\f\r"
#define EXPORT_ESC "Eabtnvfr"

static void	print_quoted_value(char *str, char quote)
{
	char	*esc;

	printf("%c", quote);
	while (*str)
	{
		esc = ft_strchr(EXPORT_CTRL, *str);
		if (quote == '"' && ft_strchr("\"\\$`", *str))
			printf("\\%c", *str);
		else if (quote == '\'' && esc)
			printf("\\%c", EXPORT_ESC[esc - EXPORT_CTRL]);
		else if (quote == '\'' && !ft_isprint((unsigned char)*str))
			printf("\\%03o", (unsigned char)*str);
		else if (quote == '\'' && (*str == '\\' || *str == '\''))
			printf("\\%c", *str);
		else
			printf("%c", *str);
		str++;
	}
	printf("%c", quote);
}

static int	print_export(t_bucket_contents *item)
{
	char	*value;

	if (!item->data.exported)
		return (0);
	printf("declare -x %s", item->key);
	value = item->data.value;
	while (value && *value && ft_isprint((unsigned char)*value))
		value++;
	if (value && *value)
	{
		printf("=$");
		print_quoted_value(item->data.value, '\'');
	}
	else if (value)
	{
		printf("=");
		print_quoted_value(item->data.value, '"');
	}
	printf("\n");
	return (0);
}

static t_status	set_export_value(t_export_arg *arg, t_bucket_contents *item)
{
	char	*value;

	value = ft_strdup(arg->word->str + arg->value_pos);
	if (!value)
		return (ST_FATAL);
	if (arg->append && item->data.value)
	{
		value = ft_strjoin_free(item->data.value, value, 1 << 1);
		if (!value)
			return (ST_FATAL);
	}
	free(item->data.value);
	item->data.value = value;
	return (ST_OK);
}

static t_status	put_export(t_word *wd, t_ctx *ctx)
{
	t_bucket_contents	*item;
	char				*key;
	t_export_arg		arg;

	arg.word = wd;
	arg.key_len = str_name_len(wd->str);
	arg.value_pos = str_assign_pos(wd->str, &arg.append);
	if (!arg.key_len || (!arg.value_pos && wd->str[arg.key_len]))
		return (ST_FAILURE);
	if (arg.value_pos)
		arg.value_pos++;
	key = ft_strndup(wd->str, arg.key_len);
	if (!key)
		return (ST_FATAL);
	item = hash_insert(key, ctx->env_table);
	free(key);
	if (!item)
		return (ST_FATAL);
	item->data.exported = true;
	if (arg.value_pos > 0)
		return (set_export_value(&arg, item));
	return (ST_OK);
}

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
	t_status	arg_status;
	t_status	status;

	if (!args)
	{
		hash_walk(ctx->env_table, print_export);
		return (ST_OK);
	}
	status = ST_OK;
	while (args)
	{
		arg_status = put_export(args->wd, ctx);
		if (arg_status == ST_FAILURE)
		{
			ft_putendl_fd("minishell: export: not a valid identifier",
				STDERR_FILENO);
			status = ST_FAILURE;
		}
		else if (arg_status == ST_FATAL)
			return (ST_FATAL);
		args = args->next;
	}
	return (status);
}
