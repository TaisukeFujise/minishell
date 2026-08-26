/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/25 20:26:53 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/signal_handle.h"
#include "../include/execute.h"
#include "../include/parser.h"

void	dispose_shell(char *user_input, t_ctx *ctx)
{
	free(user_input);
	if (ctx->env_table != NULL)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
		ctx->env_table = NULL;
	}
	if (ctx->tmp_table != NULL)
	{
		hash_flush(ctx->tmp_table, NULL);
		hash_dispose(ctx->tmp_table);
		ctx->tmp_table = NULL;
	}
	rl_clear_history();
}

static t_status	run_command(char **cursor, t_node *ast, t_ctx *ctx,
		t_arenas *arenas)
{
	t_status	status;

	status = parse(cursor, ast, ctx, arenas);
	if (status == ST_OK)
		status = execute(ast, ctx);
	close_heredocs(ast->left);
	ast->left = NULL;
	ctx->arenas = NULL;
	ft_arena_destroy(&arenas->tmp);
	ft_arena_destroy(&arenas->ast);
	return (status);
}

static void	parse_and_execute(char *input, t_node *ast, t_ctx *ctx)
{
	char		*cursor;
	t_status	status;
	t_arenas	arenas;

	cursor = input;
	while (*cursor != '\0')
	{
		ft_arena_init(&arenas.ast, ARENA_DEFAULT_CHUNK_SIZE);
		ft_arena_init(&arenas.tmp, ARENA_DEFAULT_CHUNK_SIZE);
		ctx->arenas = &arenas;
		status = run_command(&cursor, ast, ctx, &arenas);
		if (status == ST_EXIT || status == ST_FATAL)
		{
			if (status == ST_FATAL && ctx->err.exit_code == 0)
				ctx->err.exit_code = 1;
			dispose_shell(input, ctx);
			exit(ctx->err.exit_code);
		}
	}
}

static void	shell_loop(t_ctx *ctx)
{
	char		*input;
	t_node		ast;

	while (1)
	{
		g_signum = 0;
		input = shell_read_line("minishell$ ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		if (g_signum == SIGINT)
			ctx->err.exit_code = 130;
		parse_and_execute(input, &ast, ctx);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	if (set_signal() == FAILURE)
		return (1);
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	shell_loop(&ctx);
	dispose_shell(NULL, &ctx);
	return (ctx.err.exit_code);
}
