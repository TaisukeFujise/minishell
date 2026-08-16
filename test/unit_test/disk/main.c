/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:33:14 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/15 15:59:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disk_test.h"

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

static t_word_list	*new_args_1(char *arg0)
{
	t_word_list	*args;

	args = malloc(sizeof(t_word_list));
	if (args == NULL)
		return (NULL);
	args->wd = new_word(arg0);
	if (args->wd == NULL)
		return (free(args), NULL);
	args->next = NULL;
	return (args);
}

static void	free_args_1(t_word_list *args)
{
	if (args == NULL)
		return ;
	if (args->wd != NULL)
	{
		free(args->wd->str);
		free(args->wd);
	}
	free(args);
}

static int	run_disk_command_and_wait(t_ctx *ctx, char *arg0)
{
	t_simple_cmd	cmd;

	ft_bzero(&cmd, sizeof(t_simple_cmd));
	cmd.args = new_args_1(arg0);
	assert(cmd.args != NULL);
	assert(exec_disk_command(&cmd, ctx, NO_PIPE, NO_PIPE) == ST_OK);
	assert(ctx->already_forked == 1);
	assert(ctx->npid == 1);
	assert(collect_child_result(ctx) == ST_OK);
	free_args_1(cmd.args);
	return (ctx->err.exit_code);
}

static int	run_test_case(t_ctx *ctx, char *label, char *arg0)
{
	int	exit_code;

	exit_code = run_disk_command_and_wait(ctx, arg0);
	if (exit_code == 0)
	{
		printf("[OK] %s: exit_code=%d\n", label, exit_code);
		return (0);
	}
	printf("[NG] %s: exit_code=%d\n", label, exit_code);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;
	int		ng_count;

	(void)argc;
	(void)argv;
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	ng_count = 0;
	ng_count += run_test_case(&ctx, "command with slash", "/bin/ls");
	ng_count += run_test_case(&ctx, "command without slash", "ls");
	if (ng_count == 0)
		return (0);
	return (1);
}
