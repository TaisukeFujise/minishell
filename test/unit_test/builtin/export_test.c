/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 00:22:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 00:00:00 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"

static int	g_ng;

static bool	has_value(t_ctx *ctx, char *key, char *value)
{
	t_bucket_contents	*item;

	item = hash_search(key, ctx->env_table);
	return (item && item->data.exported && item->data.value
		&& ft_strcmp(item->data.value, value) == 0);
}

static t_status	run_export(t_ctx *ctx, char **strs)
{
	t_word_list	*args;
	t_status	status;

	args = make_args(strs);
	if (!args)
		return (ST_FATAL);
	status = export_cmd(args, ctx);
	free_args(args);
	return (status);
}

int	test_export(char **envp)
{
	t_ctx	ctx;

	g_ng = 0;
	if (setup_ctx(&ctx, envp))
		return (1);
	CHECK(run_export(&ctx, (char *[]){"MS53_NAME", NULL}) == ST_OK
		&& hash_search("MS53_NAME", ctx.env_table) != NULL,
		"export: plain name with W_NONE");
	CHECK(run_export(&ctx, (char *[]){"MS53_A=value", NULL}) == ST_OK
		&& has_value(&ctx, "MS53_A", "value"),
		"export: assignment with W_NONE");
	CHECK(run_export(&ctx, (char *[]){"MS53_A+=tail", NULL}) == ST_OK
		&& has_value(&ctx, "MS53_A", "valuetail"),
		"export: append assignment with W_NONE");
	CHECK(run_export(&ctx, (char *[]){"1BAD", "MS53_B=ok", NULL})
		== ST_FAILURE && has_value(&ctx, "MS53_B", "ok"),
		"export: continue after invalid identifier");
	teardown_ctx(&ctx);
	return (g_ng);
}
