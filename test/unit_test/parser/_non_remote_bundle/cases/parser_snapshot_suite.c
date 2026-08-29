#include "parser_test.h"
#include "parser_snapshot_suite.h"

static char	*parse_dump_once(const char *input)
{
	t_ctx		ctx;
	t_node		ast;
	t_status	status;
	char		*cursor;
	char		*dump;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = (char *)input;
	status = parse(&cursor, &ast, &ctx);
	if (status != ST_OK)
	{
		return (NULL);
	}
	dump = parser_dump_ast_to_string(&ast);
	free_node(&ast);
	return (dump);
}

static char	*parse_dump_complete_chain(const char *input)
{
	t_ctx		ctx;
	t_node		ast1;
	t_node		ast2;
	t_status	status;
	char		*cursor;
	char		*dump;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = (char *)input;
	status = parse(&cursor, &ast1, &ctx);
	if (status != ST_OK)
		return (NULL);
	status = parse(&cursor, &ast2, &ctx);
	if (status != ST_OK)
		return (free_node(&ast1), NULL);
	ast1.right = &ast2;
	dump = parser_dump_ast_to_string(&ast1);
	free_node(&ast1);
	return (dump);
}

static int	assert_snapshot(const char *name, char *got, const char *expected)
{
	if (!got)
		return (printf("%s: failed to generate dump\n", name), 0);
	if (ft_strcmp(got, expected) != 0)
	{
		printf("%s: snapshot mismatch\n", name);
		printf("---- expected ----\n%s", expected);
		printf("---- got ----\n%s", got);
		parser_dump_free_string(got);
		return (0);
	}
	parser_dump_free_string(got);
	return (1);
}

static int	test_simple(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │  ├─ left: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  ├─ args[0]: word=\"echo\" flags=ID\n"
		"   │  └─ args[1]: word=\"a\" flags=ID\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("echo a");
	return (assert_snapshot("simple", dump, expected));
}

static int	test_complete_chain(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │  ├─ left: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  ├─ args[0]: word=\"echo\" flags=ID\n"
		"   │  └─ args[1]: word=\"a\" flags=ID\n"
		"   └─ right: NODE_COMPLETE\n"
		"      ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"      │  ├─ left: NULL\n"
		"      │  ├─ right: NULL\n"
		"      │  ├─ args[0]: word=\"echo\" flags=ID\n"
		"      │  └─ args[1]: word=\"b\" flags=ID\n"
		"      └─ right: NULL\n";

	dump = parse_dump_complete_chain("echo a\necho b\n");
	return (assert_snapshot("complete_chain", dump, expected));
}

static int	test_andor(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_ANDOR op=&&\n"
		"   │  ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │  │  ├─ left: NULL\n"
		"   │  │  ├─ right: NULL\n"
		"   │  │  ├─ args[0]: word=\"echo\" flags=ID\n"
		"   │  │  └─ args[1]: word=\"a\" flags=ID\n"
		"   │  └─ right: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │     ├─ left: NULL\n"
		"   │     ├─ right: NULL\n"
		"   │     ├─ args[0]: word=\"echo\" flags=ID\n"
		"   │     └─ args[1]: word=\"b\" flags=ID\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("echo a && echo b");
	return (assert_snapshot("andor", dump, expected));
}

static int	test_pipeline(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_PIPE op=|\n"
		"   │  ├─ left: NODE_PIPE op=|\n"
		"   │  │  ├─ left: NODE_SIMPLE assigns=0 args=1 redirects=0\n"
		"   │  │  │  ├─ left: NULL\n"
		"   │  │  │  ├─ right: NULL\n"
		"   │  │  │  └─ args[0]: word=\"a\" flags=ID\n"
		"   │  │  └─ right: NODE_SIMPLE assigns=0 args=1 redirects=0\n"
		"   │  │     ├─ left: NULL\n"
		"   │  │     ├─ right: NULL\n"
		"   │  │     └─ args[0]: word=\"b\" flags=ID\n"
		"   │  └─ right: NODE_SIMPLE assigns=0 args=1 redirects=0\n"
		"   │     ├─ left: NULL\n"
		"   │     ├─ right: NULL\n"
		"   │     └─ args[0]: word=\"c\" flags=ID\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("a | b | c");
	return (assert_snapshot("pipeline", dump, expected));
}

static int	test_subshell_redirect(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SUBSHELL redirects=1\n"
		"   │  ├─ left: NODE_COMPLETE\n"
		"   │  │  ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │  │  │  ├─ left: NULL\n"
		"   │  │  │  ├─ right: NULL\n"
		"   │  │  │  ├─ args[0]: word=\"echo\" flags=ID\n"
		"   │  │  │  └─ args[1]: word=\"hi\" flags=ID\n"
		"   │  │  └─ right: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  └─ redirects[0]: op=> io=1 target=\"out\"\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("(echo hi) >out");
	return (assert_snapshot("subshell_redirect", dump, expected));
}

static int	test_assigns_vs_args(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SIMPLE assigns=2 args=2 redirects=0\n"
		"   │  ├─ left: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  ├─ assigns[0]: key=\"A\" value=\"1\" key_flags=ASSIGN\n"
		"   │  ├─ assigns[1]: key=\"B\" value=\"2\" key_flags=APPEND\n"
		"   │  ├─ args[0]: word=\"cmd\" flags=ID\n"
		"   │  └─ args[1]: word=\"x\" flags=ID\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("A=1 B+=2 cmd x");
	return (assert_snapshot("assigns_prefix", dump, expected));
}

static int	test_assigns_as_args(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │  ├─ left: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  ├─ args[0]: word=\"cmd\" flags=ID\n"
		"   │  └─ args[1]: word=\"A=1\" flags=ASSIGN\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("cmd A=1");
	return (assert_snapshot("assigns_as_args", dump, expected));
}

static int	test_heredoc(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SIMPLE assigns=0 args=1 redirects=1\n"
		"   │  ├─ left: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  ├─ args[0]: word=\"cat\" flags=ID\n"
		"   │  └─ redirects[0]: op=<< io=0 target=\"eof\" hd_len=2 hd_preview=\"x\\n\"\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("cat << eof\nx\neof\n");
	return (assert_snapshot("heredoc", dump, expected));
}

static int	test_null_root(void)
{
	char	*dump;

	dump = parser_dump_ast_to_string(NULL);
	return (assert_snapshot("null_root", dump, "NULL\n"));
}

static int	test_escape_stability(void)
{
	char		*dump;
	const char	*expected =
		"└─ NODE_COMPLETE\n"
		"   ├─ left: NODE_SIMPLE assigns=0 args=2 redirects=0\n"
		"   │  ├─ left: NULL\n"
		"   │  ├─ right: NULL\n"
		"   │  ├─ args[0]: word=\"echo\" flags=ID\n"
		"   │  └─ args[1]: word=\"a\\tb\\\\\\\\c\" flags=DQ\n"
		"   └─ right: NULL\n";

	dump = parse_dump_once("echo \"a\tb\\\\c\"");
	return (assert_snapshot("escape_stability", dump, expected));
}

int	parser_snapshot_run(void)
{
	int	pass;
	int	total;

	pass = 0;
	total = 10;
	pass += test_simple();
	pass += test_complete_chain();
	pass += test_andor();
	pass += test_pipeline();
	pass += test_subshell_redirect();
	pass += test_assigns_vs_args();
	pass += test_assigns_as_args();
	pass += test_heredoc();
	pass += test_null_root();
	pass += test_escape_stability();
	printf("dump snapshot: %d/%d passed\n", pass, total);
	return (pass != total);
}
