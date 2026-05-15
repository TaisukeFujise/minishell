/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 00:22:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 00:00:00 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"
#include <string.h>

static int	g_ng;

static void	insert_tmp(t_ctx *ctx, char *key, char *value, bool exported)
{
	t_bucket_contents	*item;

	item = hash_insert(ft_strdup(key), ctx->tmp_table);
	item->data.value = ft_strdup(value);
	item->data.exported = exported;
}

int	test_env(char **envp)
{
	t_ctx		ctx;
	char		buf[4096];
	t_word_list	*args;

	g_ng = 0;
	if (setup_ctx(&ctx, envp))
		return (1);
	CHECK(capture(buf, sizeof(buf), env_cmd, NULL, &ctx) == ST_OK && strstr(buf,
			"PATH=") != NULL, "env: no args");
	args = make_args((char *[]){"x", NULL});
	CHECK(capture(buf, sizeof(buf), env_cmd, args, &ctx) == ST_FAILURE,
		"env: with args");
	free_args(args);
	teardown_ctx(&ctx);
	if (setup_ctx(&ctx, envp))
		return (1);
	ctx.tmp_table = hash_create(BUCKET_SIZE);
	if (ctx.tmp_table == NULL)
		return (teardown_ctx(&ctx), 1);
	insert_tmp(&ctx, "PATH", "/tmp/override", true);
	CHECK(capture(buf, sizeof(buf), env_cmd, NULL, &ctx) == ST_OK
		&& strstr(buf, "PATH=/tmp/override") != NULL,
		"env: tmp_table overrides env_table");
	insert_tmp(&ctx, "TMPONLY", "42", true);
	CHECK(capture(buf, sizeof(buf), env_cmd, NULL, &ctx) == ST_OK
		&& strstr(buf, "TMPONLY=42") != NULL,
		"env: tmp_table-only var appears");
	insert_tmp(&ctx, "UNEXPORTED", "hidden", false);
	CHECK(capture(buf, sizeof(buf), env_cmd, NULL, &ctx) == ST_OK
		&& strstr(buf, "UNEXPORTED") == NULL,
		"env: unexported tmp var not shown");
	teardown_ctx(&ctx);
	return (g_ng);
}
