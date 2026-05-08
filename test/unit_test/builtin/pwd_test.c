#include "builtin_test.h"

static int	g_ng;

int	test_pwd(char **envp)
{
	char	buf[1024];
	char	expected[1024];
	char	*cwd;

	(void)envp;
	g_ng = 0;

	cwd = getcwd(NULL, 0);
	ft_strlcpy(expected, cwd, sizeof(expected));
	ft_strlcat(expected, "\n", sizeof(expected));
	free(cwd);
	CHECK(capture(buf, sizeof(buf), pwd_cmd, NULL, NULL) == ST_OK
		&& ft_strcmp(buf, expected) == 0, "pwd: no args");

	CHECK(capture(buf, sizeof(buf), pwd_cmd,
			make_args((char *[]){"x", NULL}), NULL) == ST_FAILURE,
		"pwd: with args");

	return (g_ng);
}
