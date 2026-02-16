/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:31:00 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 02:31:00 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connection_pipe_test.h"

static t_word	*new_word(char *str)
{
	t_word	*word;

	word = ft_calloc(1, sizeof(t_word));
	if (word == NULL)
		return (NULL);
	word->str = ft_strdup(str);
	if (word->str == NULL)
		return (free(word), NULL);
	word->len = ft_strlen(str);
	return (word);
}

static t_word_list	*new_args(int argc, char **argv)
{
	t_word_list	*head;
	t_word_list	*cur;
	t_word_list	*new_node;
	int			i;

	if (argc < 1)
		return (NULL);
	head = NULL;
	cur = NULL;
	i = 0;
	while (i < argc)
	{
		new_node = ft_calloc(1, sizeof(t_word_list));
		if (new_node == NULL)
			return (head);
		new_node->wd = new_word(argv[i]);
		if (new_node->wd == NULL)
			return (head);
		if (head == NULL)
			head = new_node;
		else
			cur->next = new_node;
		cur = new_node;
		i++;
	}
	return (head);
}

static t_node	*new_simple_node(int argc, char **argv)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (node == NULL)
		return (NULL);
	node->node_kind = NODE_SIMPLE;
	node->u_node.simple_command.args = new_args(argc, argv);
	return (node);
}

static t_node	*new_pipe_node(t_node *left, t_node *right)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (node == NULL)
		return (NULL);
	node->node_kind = NODE_PIPE;
	node->left = left;
	node->right = right;
	return (node);
}

static void	free_args(t_word_list *args)
{
	t_word_list	*next;

	while (args)
	{
		next = args->next;
		if (args->wd != NULL)
		{
			free(args->wd->str);
			free(args->wd);
		}
		free(args);
		args = next;
	}
}

static void	free_node_tree(t_node *node)
{
	if (node == NULL)
		return ;
	free_node_tree(node->left);
	free_node_tree(node->right);
	if (node->node_kind == NODE_SIMPLE)
		free_args(node->u_node.simple_command.args);
	free(node);
}

static t_status	exec_and_capture(t_node *root, t_ctx *ctx,
		char *buf, size_t bufsize)
{
	int			cap[2];
	int			saved_stdout;
	ssize_t		nread;
	t_status	status;

	if (pipe(cap) < 0)
		return (ST_FATAL);
	fflush(NULL);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout < 0)
		return (close(cap[0]), close(cap[1]), ST_FATAL);
	if (dup2(cap[1], STDOUT_FILENO) < 0)
		return (close(cap[0]), close(cap[1]), close(saved_stdout), ST_FATAL);
	close(cap[1]);
	status = execute(root, ctx);
	fflush(NULL);
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		return (close(cap[0]), close(saved_stdout), ST_FATAL);
	close(saved_stdout);
	nread = read(cap[0], buf, bufsize - 1);
	close(cap[0]);
	if (nread < 0)
		return (ST_FATAL);
	buf[nread] = '\0';
	return (status);
}

static int	run_pipeline_case(char *label, t_node *root,
		int expected_exit_code, char *expected_stdout, char **envp)
{
	t_ctx		ctx;
	t_status	status;
	char		outbuf[256];

	if (init_ctx(&ctx, envp) == FAILURE)
		return (fprintf(stderr, "[NG] %s: init_ctx failed\n", label), 1);
	status = exec_and_capture(root, &ctx, outbuf, sizeof(outbuf));
	if (status != ST_OK)
		return (fprintf(stderr, "[NG] %s: status=%d\n", label, status), 1);
	if (ctx.err.exit_code != expected_exit_code)
		return (fprintf(stderr, "[NG] %s: exit_code=%d expected=%d\n",
				label, ctx.err.exit_code, expected_exit_code), 1);
	if (expected_stdout != NULL && ft_strcmp(outbuf, expected_stdout) != 0)
		return (fprintf(stderr, "[NG] %s: stdout='%s' expected='%s'\n",
				label, outbuf, expected_stdout), 1);
	fprintf(stderr, "[OK] %s\n", label);
	return (0);
}

static int	test_two_command_success(char **envp)
{
	t_node	*root;
	t_node	*c1;
	t_node	*c2;
	char	*argv1[] = {"/bin/echo", "hello"};
	char	*argv2[] = {"/bin/cat"};

	c1 = new_simple_node(2, argv1);
	c2 = new_simple_node(1, argv2);
	root = new_pipe_node(c1, c2);
	if (root == NULL)
		return (1);
	if (run_pipeline_case("two command success",
			root, 0, "hello\n", envp) != 0)
		return (free_node_tree(root), 1);
	free_node_tree(root);
	return (0);
}

static int	test_three_command_success(char **envp)
{
	t_node	*root;
	t_node	*left_pipe;
	t_node	*c1;
	t_node	*c2;
	t_node	*c3;
	char	*argv1[] = {"/bin/echo", "hello"};
	char	*argv2[] = {"/usr/bin/tr", "a-z", "A-Z"};
	char	*argv3[] = {"/bin/cat"};

	c1 = new_simple_node(2, argv1);
	c2 = new_simple_node(3, argv2);
	c3 = new_simple_node(1, argv3);
	left_pipe = new_pipe_node(c1, c2);
	root = new_pipe_node(left_pipe, c3);
	if (root == NULL)
		return (1);
	if (run_pipeline_case("three command success",
			root, 0, "HELLO\n", envp) != 0)
		return (free_node_tree(root), 1);
	free_node_tree(root);
	return (0);
}

static int	test_first_command_not_found(char **envp)
{
	t_node	*root;
	t_node	*c1;
	t_node	*c2;
	char	*argv1[] = {"nosuchcmd"};
	char	*argv2[] = {"/bin/cat"};

	c1 = new_simple_node(1, argv1);
	c2 = new_simple_node(1, argv2);
	root = new_pipe_node(c1, c2);
	if (root == NULL)
		return (1);
	if (run_pipeline_case("first command not found",
			root, 0, "", envp) != 0)
		return (free_node_tree(root), 1);
	free_node_tree(root);
	return (0);
}

static int	test_last_command_not_found(char **envp)
{
	t_node	*root;
	t_node	*c1;
	t_node	*c2;
	char	*argv1[] = {"/bin/echo", "hello"};
	char	*argv2[] = {"nosuchcmd"};

	c1 = new_simple_node(2, argv1);
	c2 = new_simple_node(1, argv2);
	root = new_pipe_node(c1, c2);
	if (root == NULL)
		return (1);
	if (run_pipeline_case("last command not found",
			root, 127, NULL, envp) != 0)
		return (free_node_tree(root), 1);
	free_node_tree(root);
	return (0);
}

static int	test_empty_input_pipeline(char **envp)
{
	t_node	*root;
	t_node	*c1;
	t_node	*c2;
	char	*argv1[] = {"/usr/bin/printf", ""};
	char	*argv2[] = {"/bin/cat"};

	c1 = new_simple_node(2, argv1);
	c2 = new_simple_node(1, argv2);
	root = new_pipe_node(c1, c2);
	if (root == NULL)
		return (1);
	if (run_pipeline_case("empty input pipeline",
			root, 0, "", envp) != 0)
		return (free_node_tree(root), 1);
	free_node_tree(root);
	return (0);
}

/*
	Executed test cases
	1. two command success
		- /bin/echo hello | /bin/cat
		- expect: exit_code=0, stdout="hello\n"
	2. three command success
		- /bin/echo hello | /usr/bin/tr a-z A-Z | /bin/cat
		- expect: exit_code=0, stdout="HELLO\n"
	3. first command not found
		- nosuchcmd | /bin/cat
		- expect: exit_code=0, stdout=""
	4. last command not found
		- /bin/echo hello | nosuchcmd
		- expect: exit_code=127
	5. empty input pipeline
		- /usr/bin/printf "" | /bin/cat
		- expect: exit_code=0, stdout=""
*/
int	main(int argc, char **argv, char **envp)
{
	int	ng_count;

	(void)argc;
	(void)argv;
	ng_count = 0;
	ng_count += test_two_command_success(envp);
	ng_count += test_three_command_success(envp);
	ng_count += test_first_command_not_found(envp);
	ng_count += test_last_command_not_found(envp);
	ng_count += test_empty_input_pipeline(envp);
	if (ng_count == 0)
		return (0);
	return (1);
}
