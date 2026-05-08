#include "builtin_test.h"
#include <string.h>

static int	g_ng;

int	test_env(char **envp)
{
	t_ctx		ctx;
	char		buf[4096];
	t_word_list	*args;

	g_ng = 0;
	if (setup_ctx(&ctx, envp))
		return (1);

	CHECK(capture(buf, sizeof(buf), env_cmd, NULL, &ctx) == ST_OK
		&& strstr(buf, "PATH=") != NULL, "env: no args");

	args = make_args((char *[]){"x", NULL});
	CHECK(capture(buf, sizeof(buf), env_cmd, args, &ctx) == ST_FAILURE,
		"env: with args");
	free_args(args);

	teardown_ctx(&ctx);
	return (g_ng);
}
