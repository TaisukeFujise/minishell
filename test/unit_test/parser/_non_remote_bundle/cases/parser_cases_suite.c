#include "parser_test.h"
#include "parser_cases_suite.h"

static char	*flatten_word(t_word *word)
{
	size_t	len;
	char	*buf;
	char	*cur;
	t_word	*head;

	len = 0;
	head = word;
	while (word)
	{
		len += word->len;
		word = word->next;
	}
	buf = ft_calloc(len + 1, sizeof(char));
	if (!buf)
		return (NULL);
	cur = buf;
	word = head;
	while (word)
	{
		ft_memcpy(cur, word->str, word->len);
		cur += word->len;
		word = word->next;
	}
	return (buf);
}

static int	word_equals(t_word *word, const char *expect)
{
	char	*str;
	int		ok;

	str = flatten_word(word);
	if (!str)
		return (0);
	ok = (ft_strcmp(str, expect) == 0);
	free(str);
	return (ok);
}

static int	count_complete_nodes(t_node *node)
{
	int	count;

	if (!node)
		return (0);
	if (node->node_kind != NODE_COMPLETE)
		return (1);
	count = 0;
	while (node && node->node_kind == NODE_COMPLETE)
	{
		count++;
		node = node->right;
	}
	return (count);
}

static t_node	*leftmost_exec_node(t_node *node)
{
	if (!node)
		return (NULL);
	if (node->node_kind == NODE_COMPLETE)
		return (leftmost_exec_node(node->left));
	if (node->node_kind == NODE_ANDOR || node->node_kind == NODE_PIPE)
		return (leftmost_exec_node(node->left));
	return (node);
}

static int	count_assigns(t_assign *assigns)
{
	int	count;

	count = 0;
	while (assigns)
	{
		count++;
		assigns = assigns->next;
	}
	return (count);
}

static int	count_args(t_word_list *args)
{
	int	count;

	count = 0;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

static t_word	*arg_at(t_word_list *args, int index)
{
	int	i;

	i = 0;
	while (args)
	{
		if (i == index)
			return (args->wd);
		args = args->next;
		i++;
	}
	return (NULL);
}

static t_redirect	*redir_at(t_redirect *redirects, int index)
{
	int	i;

	i = 0;
	while (redirects)
	{
		if (i == index)
			return (redirects);
		redirects = redirects->next;
		i++;
	}
	return (NULL);
}

static int	check_simple_layout(char *input, int assigns, int args)
{
	t_ctx	ctx;
	t_node	ast;
	t_node	*node;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	cursor = input;
	parser_mock_set_lines(NULL);
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	if (count_assigns(node->u_node.simple_command.assigns) != assigns)
		return (free_node(ast.left), 0);
	if (count_args(node->u_node.simple_command.args) != args)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	run_parse_loop(char *input, int *ok_units, int *fail_units,
	int *complete_nodes)
{
	t_ctx		ctx;
	t_node		ast;
	t_status	status;
	char		*cursor;
	char		*prev;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = input;
	while (*cursor)
	{
		prev = cursor;
		status = parse(&cursor, &ast, &ctx);
		if (status == ST_OK)
		{
			if (ast.left)
			{
				(*ok_units)++;
				*complete_nodes += count_complete_nodes(ast.left);
			}
			free_node(ast.left);
		}
		else if (status == ST_FAILURE)
			(*fail_units)++;
		else
			return (0);
		if (cursor == prev)
			return (0);
	}
	return (1);
}

static int	case_01(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo a";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	if (!ast.left || count_complete_nodes(ast.left) != 1)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_02(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("echo a\necho b\n", &ok_units, &fail_units, &complete_nodes))
		return (0);
	return (fail_units == 0 && complete_nodes == 2);
}

static int	case_03(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;
	t_node	*root;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo a &&\necho b\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	root = ast.left;
	if (!root || root->node_kind != NODE_ANDOR
		|| root->u_node.and_or.op != CONNECT_AND_IF)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_04(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;
	t_status	status;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo a &&";
	status = parse(&cursor, &ast, &ctx);
	return (status == ST_FAILURE && ctx.err.exit_code == 2);
}

static int	case_05(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo \"a\nb\"\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	free_node(ast.left);
	return (1);
}

static int	case_06(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo \"a";
	return (parse(&cursor, &ast, &ctx) == ST_FAILURE);
}

static int	case_07(void)
{
	t_ctx		ctx;
	t_node		ast;
	char		*cursor;
	t_node		*node;
	t_redirect	*redir;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cat << eof\nx\neof\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	redir = node->u_node.simple_command.redirects;
	if (!redir || redir->op != REDIR_DLESS || !redir->hd.raw_str.str)
		return (free_node(ast.left), 0);
	if (ft_strcmp(redir->hd.raw_str.str, "x\n") != 0)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_08(void)
{
	const char	*lines[] = {"eof", NULL};
	t_ctx		ctx;
	t_node		ast;
	char		*cursor;
	t_node		*node;
	t_redirect	*redir;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(lines);
	cursor = "cat << eof\nx\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	redir = node->u_node.simple_command.redirects;
	if (!redir || !redir->hd.raw_str.str)
		return (free_node(ast.left), 0);
	if (ft_strcmp(redir->hd.raw_str.str, "x\n") != 0)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_09(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("echo before\n)\necho after\n", &ok_units, &fail_units,
			&complete_nodes))
		return (0);
	return (fail_units == 1 && complete_nodes == 2);
}

static int	case_10(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = ")";
	if (parse(&cursor, &ast, &ctx) != ST_FAILURE)
		return (0);
	return (ctx.err.exit_code != 0);
}

static int	case_11(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_simple_cmd	*cmd;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "A=1 B+=2 cmd x";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	cmd = &node->u_node.simple_command;
	if (count_assigns(cmd->assigns) != 2 || count_args(cmd->args) != 2)
		return (free_node(ast.left), 0);
	if (!word_equals(arg_at(cmd->args, 0), "cmd")
		|| !word_equals(arg_at(cmd->args, 1), "x"))
		return (free_node(ast.left), 0);
	if (!(cmd->assigns->next->key->flag & W_APPEND))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_12(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_simple_cmd	*cmd;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cmd A=1 B+=2";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	cmd = &node->u_node.simple_command;
	if (count_assigns(cmd->assigns) != 0 || count_args(cmd->args) != 3)
		return (free_node(ast.left), 0);
	if (!word_equals(arg_at(cmd->args, 1), "A=1")
		|| !word_equals(arg_at(cmd->args, 2), "B+=2"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_13(void)
{
	return (check_simple_layout("export A=1 B+=2", 0, 3));
}

static int	case_14(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "\n\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	return (ast.left == NULL && *cursor == '\0');
}

static int	case_15(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_redirect	*redir;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "(echo hi) >out";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SUBSHELL)
		return (free_node(ast.left), 0);
	redir = node->u_node.subshell.redirects;
	if (!redir || redir->op != REDIR_GREATER)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_16(void)
{
	return (case_09());
}

static int	case_17(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "2>>";
	return (parse(&cursor, &ast, &ctx) == ST_FAILURE);
}

static int	case_18(void)
{
	if (!check_simple_layout("A=1 >out cmd", 1, 1))
		return (0);
	return (1);
}

static int	case_19(void)
{
	if (!check_simple_layout("cmd A=1 >out", 0, 2))
		return (0);
	return (1);
}

static int	case_20(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_redirect	*redir;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cat << eof\n\nx\neof\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	redir = redir_at(node->u_node.simple_command.redirects, 0);
	if (!redir || redir->op != REDIR_DLESS || !redir->hd.raw_str.str)
		return (free_node(ast.left), 0);
	if (ft_strcmp(redir->hd.raw_str.str, "\nx\n") != 0)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_21(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_redirect	*first;
	t_redirect	*second;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cat << a << b\nX\na\nY\nb\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	first = redir_at(node->u_node.simple_command.redirects, 0);
	second = redir_at(node->u_node.simple_command.redirects, 1);
	if (!first || !second || second->next)
		return (free_node(ast.left), 0);
	if (ft_strcmp(first->hd.raw_str.str, "X\n") != 0
		|| ft_strcmp(second->hd.raw_str.str, "Y\n") != 0)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_22(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_status	status;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cat << eof |\nwc\neof\n";
	status = parse(&cursor, &ast, &ctx);
	return (status == ST_FAILURE && ctx.err.exit_code == 2);
}

static int	case_23(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_status	status;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cat << eof &&\nwc\neof\n";
	status = parse(&cursor, &ast, &ctx);
	return (status == ST_FAILURE && ctx.err.exit_code == 2);
}

static int	case_24(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*subshell;
	t_node		*first;
	t_node		*second;
	t_redirect	*redir;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "(cat << eof\nx\neof\necho ok\n)\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	subshell = leftmost_exec_node(ast.left);
	if (!subshell || subshell->node_kind != NODE_SUBSHELL)
		return (free_node(ast.left), 0);
	if (!subshell->left || subshell->left->node_kind != NODE_COMPLETE)
		return (free_node(ast.left), 0);
	first = leftmost_exec_node(subshell->left->left);
	second = subshell->left->right;
	if (!first || first->node_kind != NODE_SIMPLE || !second)
		return (free_node(ast.left), 0);
	redir = redir_at(first->u_node.simple_command.redirects, 0);
	if (!redir || !redir->hd.raw_str.str || ft_strcmp(redir->hd.raw_str.str, "x\n"))
		return (free_node(ast.left), 0);
	second = leftmost_exec_node(second->left);
	if (!second || second->node_kind != NODE_SIMPLE
		|| !word_equals(arg_at(second->u_node.simple_command.args, 0), "echo")
		|| !word_equals(arg_at(second->u_node.simple_command.args, 1), "ok"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_25(void)
{
	const char	*lines[] = {"eof", NULL};
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_redirect	*redir;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(lines);
	cursor = "cat << eof";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	redir = redir_at(node->u_node.simple_command.redirects, 0);
	if (!redir || redir->op != REDIR_DLESS || !redir->hd.raw_str.str)
		return (free_node(ast.left), 0);
	if (redir->hd.raw_str.len != 0 || redir->hd.raw_str.str[0] != '\0')
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_26(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("cat << eof\nx\neof\necho b\n", &ok_units, &fail_units,
			&complete_nodes))
		return (0);
	return (ok_units == 2 && fail_units == 0 && complete_nodes == 2);
}

static int	case_27(void)
{
	t_ctx	ctx;
	t_node	ast;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo ok";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	if (!ast.left || ast.left->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_28(void)
{
	t_ctx	ctx;
	t_node	ast;
	t_node	*subshell;
	t_node	*third;
	t_node	*cmd;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "(echo a\necho b\necho c\n)\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	subshell = leftmost_exec_node(ast.left);
	if (!subshell || subshell->node_kind != NODE_SUBSHELL
		|| count_complete_nodes(subshell->left) != 3)
		return (free_node(ast.left), 0);
	third = subshell->left->right->right;
	if (!third || third->node_kind != NODE_COMPLETE)
		return (free_node(ast.left), 0);
	cmd = leftmost_exec_node(third->left);
	if (!cmd || cmd->node_kind != NODE_SIMPLE
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 0), "echo")
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 1), "c"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_29(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("echo a\n\necho b\n", &ok_units, &fail_units,
			&complete_nodes))
		return (0);
	return (ok_units == 2 && fail_units == 0 && complete_nodes == 2);
}

static int	case_30(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("echo a\n|\necho b\n", &ok_units, &fail_units,
			&complete_nodes))
		return (0);
	return (ok_units == 2 && fail_units == 1 && complete_nodes == 2);
}

static int	case_31(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("echo a )\necho b\n", &ok_units, &fail_units,
			&complete_nodes))
		return (0);
	return (ok_units == 1 && fail_units == 1 && complete_nodes == 1);
}

static int	case_32(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_redirect	*redir;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "< in";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	if (count_assigns(node->u_node.simple_command.assigns) != 0
		|| count_args(node->u_node.simple_command.args) != 0)
		return (free_node(ast.left), 0);
	redir = redir_at(node->u_node.simple_command.redirects, 0);
	if (!redir || redir->op != REDIR_LESS || redir->io_number != 0
		|| !word_equals(&redir->target, "in"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_33(void)
{
	t_ctx	ctx;
	t_node	ast;
	t_node	*subshell;
	t_node	*second;
	t_node	*cmd;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "(echo a\n\n\necho b\n)\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	subshell = leftmost_exec_node(ast.left);
	if (!subshell || subshell->node_kind != NODE_SUBSHELL
		|| count_complete_nodes(subshell->left) != 2)
		return (free_node(ast.left), 0);
	second = subshell->left->right;
	if (!second || !second->left)
		return (free_node(ast.left), 0);
	cmd = leftmost_exec_node(second->left);
	if (!cmd || cmd->node_kind != NODE_SIMPLE
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 0), "echo")
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 1), "b"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_34(void)
{
	t_ctx	ctx;
	t_node	ast;
	t_node	*root;
	t_node	*left;
	t_node	*cmd;
	t_node	*subshell;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo a &&\n(echo a\necho b\n) &&\necho a\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	root = ast.left;
	if (!root || root->node_kind != NODE_ANDOR
		|| root->u_node.and_or.op != CONNECT_AND_IF)
		return (free_node(ast.left), 0);
	left = root->left;
	if (!left || left->node_kind != NODE_ANDOR
		|| left->u_node.and_or.op != CONNECT_AND_IF)
		return (free_node(ast.left), 0);
	cmd = leftmost_exec_node(left->left);
	if (!cmd || cmd->node_kind != NODE_SIMPLE
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 0), "echo")
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 1), "a"))
		return (free_node(ast.left), 0);
	subshell = leftmost_exec_node(left->right);
	if (!subshell || subshell->node_kind != NODE_SUBSHELL
		|| count_complete_nodes(subshell->left) != 2)
		return (free_node(ast.left), 0);
	cmd = leftmost_exec_node(root->right);
	if (!cmd || cmd->node_kind != NODE_SIMPLE
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 0), "echo")
		|| !word_equals(arg_at(cmd->u_node.simple_command.args, 1), "a"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_35(void)
{
	t_ctx	ctx;
	t_node	ast;
	t_node	*subshell;
	t_node	*first;
	t_node	*second;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "(\necho a &&\necho b\necho c ||\necho d\n)\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	subshell = leftmost_exec_node(ast.left);
	if (!subshell || subshell->node_kind != NODE_SUBSHELL
		|| count_complete_nodes(subshell->left) != 2)
		return (free_node(ast.left), 0);
	first = subshell->left;
	second = subshell->left->right;
	if (!first || !second || first->node_kind != NODE_COMPLETE
		|| second->node_kind != NODE_COMPLETE
		|| !first->left || !second->left)
		return (free_node(ast.left), 0);
	if (first->left->node_kind != NODE_ANDOR
		|| first->left->u_node.and_or.op != CONNECT_AND_IF
		|| second->left->node_kind != NODE_ANDOR
		|| second->left->u_node.and_or.op != CONNECT_OR_IF)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_36(void)
{
	int	ok_units;
	int	fail_units;
	int	complete_nodes;

	ok_units = 0;
	fail_units = 0;
	complete_nodes = 0;
	if (!run_parse_loop("echo a &\necho b\n", &ok_units, &fail_units,
			&complete_nodes))
		return (0);
	return (ok_units == 2 && fail_units == 0 && complete_nodes == 2);
}

static int	case_37(void)
{
	t_ctx	ctx;
	t_node	ast;
	t_node	*root;
	t_node	*right;
	char	*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "echo a &&\n\n\necho b\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	root = ast.left;
	if (!root || root->node_kind != NODE_ANDOR
		|| root->u_node.and_or.op != CONNECT_AND_IF)
		return (free_node(ast.left), 0);
	right = leftmost_exec_node(root->right);
	if (!right || right->node_kind != NODE_SIMPLE
		|| !word_equals(arg_at(right->u_node.simple_command.args, 0), "echo")
		|| !word_equals(arg_at(right->u_node.simple_command.args, 1), "b"))
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

static int	case_38(void)
{
	t_ctx		ctx;
	t_node		ast;
	t_node		*node;
	t_redirect	*redir;
	char		*cursor;

	ft_bzero(&ctx, sizeof(ctx));
	parser_mock_set_lines(NULL);
	cursor = "cat << e\"o\"f\nx\neof\n";
	if (parse(&cursor, &ast, &ctx) != ST_OK)
		return (0);
	node = leftmost_exec_node(ast.left);
	if (!node || node->node_kind != NODE_SIMPLE)
		return (free_node(ast.left), 0);
	redir = node->u_node.simple_command.redirects;
	if (!redir || !redir->hd.raw_str.str
		|| ft_strcmp(redir->hd.raw_str.str, "x\n") != 0)
		return (free_node(ast.left), 0);
	free_node(ast.left);
	return (1);
}

int	parser_cases_run(void)
{
	int			i;
	int			passed;
	int			total;
	int			(*cases[])(void) = {case_01, case_02, case_03, case_04, case_05,
		case_06, case_07, case_08, case_09, case_10, case_11, case_12,
		case_13, case_14, case_15, case_16, case_17, case_18, case_19,
		case_20, case_21, case_22, case_23, case_24, case_25, case_26,
		case_27, case_28, case_29, case_30, case_31, case_32, case_33,
		case_34, case_35, case_36, case_37, case_38};
	total = sizeof(cases) / sizeof(cases[0]);
	passed = 0;
	i = 0;
	while (i < total)
	{
		if (cases[i]())
			passed++;
		else
			printf("case_%02d: FAILED\n", i + 1);
		i++;
	}
	printf("parser cases: %d/%d passed\n", passed, total);
	return (passed != total);
}
