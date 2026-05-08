#include "builtin_test.h"

static int	g_ng;

int	test_echo(void)
{
	char		buf[256];
	t_word_list	*args;

	g_ng = 0;

	CHECK(capture(buf, sizeof(buf), echo_cmd, NULL, NULL) == ST_OK
		&& ft_strcmp(buf, "\n") == 0, "echo: no args");

	args = make_args((char *[]){"hello", NULL});
	CHECK(capture(buf, sizeof(buf), echo_cmd, args, NULL) == ST_OK
		&& ft_strcmp(buf, "hello\n") == 0, "echo: single arg");
	free_args(args);

	args = make_args((char *[]){"a", "b", NULL});
	CHECK(capture(buf, sizeof(buf), echo_cmd, args, NULL) == ST_OK
		&& ft_strcmp(buf, "a b\n") == 0, "echo: multi args");
	free_args(args);

	args = make_args((char *[]){"-n", "hello", NULL});
	CHECK(capture(buf, sizeof(buf), echo_cmd, args, NULL) == ST_OK
		&& ft_strcmp(buf, "hello") == 0, "echo: -n flag");
	free_args(args);

	return (g_ng);
}
