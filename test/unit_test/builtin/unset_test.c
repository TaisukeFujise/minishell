/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 00:22:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 15:09:16 by fujisetaisuke    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"

static int	g_ng;

static int	insert_var(t_hashtable *table, char *key, char *value)
{
	t_bucket_contents	*entry;
	char				*key_str;

	key_str = ft_strdup(key);
	if (key_str == NULL)
		return (1);
	entry = hash_insert(key_str, table);
	if (entry == NULL)
	{
		free(key_str);
		return (1);
	}
	entry->data.value = ft_strdup(value);
	if (entry->data.value == NULL)
		return (1);
	entry->data.exported = true;
	return (0);
}

int	test_unset(char **envp)
{
	t_ctx		ctx;
	t_word_list	*args;

	g_ng = 0;
	if (setup_ctx(&ctx, envp))
		return (1);
	CHECK(unset_cmd(NULL, &ctx) == ST_OK, "unset: no args");
	CHECK(insert_var(ctx.env_table, "TESTVAR", "hello") == 0, "insert: TESTVAR");
	args = make_args((char *[]){"TESTVAR", NULL});
	CHECK(unset_cmd(args, &ctx) == ST_OK && hash_search("TESTVAR",
			ctx.env_table) == NULL, "unset: remove existing");
	free_args(args);
	CHECK(insert_var(ctx.env_table, "AAA", "1") == 0, "insert: AAA");
	CHECK(insert_var(ctx.env_table, "BBB", "2") == 0, "insert: BBB");
	args = make_args((char *[]){"AAA", "BBB", NULL});
	CHECK(unset_cmd(args, &ctx) == ST_OK && hash_search("AAA",
			ctx.env_table) == NULL && hash_search("BBB", ctx.env_table) == NULL,
		"unset: remove multiple");
	free_args(args);
	args = make_args((char *[]){"1INVALID", NULL});
	CHECK(unset_cmd(args, &ctx) == ST_FAILURE, "unset: digit start");
	free_args(args);
	args = make_args((char *[]){"A!B", NULL});
	CHECK(unset_cmd(args, &ctx) == ST_FAILURE, "unset: invalid char");
	free_args(args);
	teardown_ctx(&ctx);
	return (g_ng);
}
