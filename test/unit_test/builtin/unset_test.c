#include "builtin_test.h"

static int	g_ng;

static void	insert_var(t_hashtable *table, char *key, char *value)
{
	t_bucket_contents	*entry;

	entry = hash_insert(ft_strdup(key), table);
	entry->data.value = ft_strdup(value);
	entry->data.exported = true;
}

int	test_unset(char **envp)
{
	t_ctx		ctx;
	t_word_list	*args;

	g_ng = 0;
	if (setup_ctx(&ctx, envp))
		return (1);
	CHECK(unset_cmd(NULL, &ctx) == ST_OK, "unset: no args");
	insert_var(ctx.env_table, "TESTVAR", "hello");
	args = make_args((char *[]){"TESTVAR", NULL});
	CHECK(unset_cmd(args, &ctx) == ST_OK && hash_search("TESTVAR",
			ctx.env_table) == NULL, "unset: remove existing");
	free_args(args);
	insert_var(ctx.env_table, "AAA", "1");
	insert_var(ctx.env_table, "BBB", "2");
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
