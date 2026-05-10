/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:31:54 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 21:54:05 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"

static t_word	*new_word(char *str)
{
	t_word	*word;

	word = malloc(sizeof(t_word));
	if (word == NULL)
		return (NULL);
	ft_bzero(word, sizeof(t_word));
	word->str = ft_strdup(str);
	if (word->str == NULL)
		return (free(word), NULL);
	word->len = ft_strlen(str);
	return (word);
}

static t_word_list	*new_args_2(char *arg0, char *arg1)
{
	t_word_list	*head;
	t_word_list	*next;

	head = ft_calloc(1, sizeof(t_word_list));
	next = ft_calloc(1, sizeof(t_word_list));
	if (head == NULL || next == NULL)
		return (free(head), free(next), NULL);
	head->wd = new_word(arg0);
	next->wd = new_word(arg1);
	if (head->wd == NULL || next->wd == NULL)
	{
		if (head->wd != NULL)
			free(head->wd->str);
		if (next->wd != NULL)
			free(next->wd->str);
		free(head->wd);
		free(next->wd);
		free(head);
		free(next);
		return (NULL);
	}
	head->next = next;
	return (head);
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

static int	test_builtin_single(t_ctx *ctx)
{
	t_simple_cmd	cmd;

	ft_bzero(&cmd, sizeof(t_simple_cmd));
	cmd.args = new_args_2("echo", "single");
	if (cmd.args == NULL)
		return (1);
	if (exec_builtin(&cmd, ctx, NO_PIPE, NO_PIPE) != ST_OK)
		return (free_args(cmd.args), 1);
	free_args(cmd.args);
	return (0);
}

static int	test_builtin_pipe(t_ctx *ctx)
{
	t_simple_cmd	cmd;
	int			fds[2];
	char		buf[64];
	ssize_t		nread;

	fflush(NULL);
	if (pipe(fds) < 0)
		return (1);
	ft_bzero(&cmd, sizeof(t_simple_cmd));
	cmd.args = new_args_2("echo", "hello");
	if (cmd.args == NULL)
		return (close(fds[0]), close(fds[1]), 1);
	if (exec_builtin(&cmd, ctx, NO_PIPE, fds[1]) != ST_OK)
		return (close(fds[0]), close(fds[1]), free_args(cmd.args), 1);
	if (collect_child_result(ctx) != ST_OK)
		return (close(fds[0]), free_args(cmd.args), 1);
	nread = read(fds[0], buf, sizeof(buf) - 1);
	if (nread < 0)
		return (close(fds[0]), free_args(cmd.args), 1);
	buf[nread] = '\0';
	close(fds[0]);
	free_args(cmd.args);
	if (ctx->err.exit_code != 0)
		return (1);
	if (ft_strcmp(buf, "hello\n") != 0)
		return (1);
	return (0);
}

/*
	Executed test cases
	1. builtin single
		- run builtin command without pipe: echo single
		- expect: exec_builtin returns ST_OK
	2. builtin pipe
		- run builtin command with pipe output: echo hello | (capture pipe)
		- expect: collect_child_result returns ST_OK
		- expect: exit_code=0, captured stdout="hello\n"
*/
int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;
	int		ng_count;
	int		result;

	(void)argc;
	(void)argv;
	int NG_count = 0;

	NG_count += test_cd(envp);
	NG_count += test_echo();
	NG_count += test_env(envp);
	NG_count += test_exit();
	NG_count += test_export(envp);
	NG_count += test_pwd(envp);
	NG_count += test_unset(envp);
	return (NG_count != 0);
}
