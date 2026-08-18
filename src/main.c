/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/02 23:48:01 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/strbuf.h"
#include "../include/signal_handle.h"
#include "../include/execute.h"
#include "../include/lexer.h"
#include "../include/parser.h"

/*
	Everything the shell itself owns: the line it is running, the tables
	of its environment and the history. The ast and the ctx are on the
	stack of main, only their members are freed. [review D37-24]
*/
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

/*
	handle_command_termination sets ctx->exit_code only when the main process cannot continue.
	A failure the shell cannot continue from keeps the status the builtin
	chose, and falls back to 1 when nothing chose one.
*/
void	handle_command_termination(t_status status, char *user_input, t_node *node, t_ctx *ctx)
{
	(void)node;
	if (status == ST_FATAL && ctx->err.exit_code == 0)
		ctx->err.exit_code = 1;
	if (status == ST_EXIT || status == ST_FATAL)
	{
		dispose_shell(user_input, ctx);
		exit(ctx->err.exit_code);
	}
}

/*
	Say what went wrong with the line. The message can live in the arena
	of this parse, so it is written before that arena goes.
*/
static void	report_error(t_ctx *ctx)
{
	if (ctx->err.err_msg == NULL)
		return ;
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, ctx->err.err_msg, ft_strlen(ctx->err.err_msg));
	write(STDERR_FILENO, "\n", 1);
	ctx->err.err_msg = NULL;
}


static t_status	parse_and_execute(char *user_input, t_node *ast, t_ctx *ctx)
{
	char		*cursor;
	t_status	status;
	t_arenas	arenas;

	cursor = user_input;
	while (*cursor != '\0')
	{
		ft_arena_init(&arenas.ast, ARENA_DEFAULT_CHUNK_SIZE);
		ft_arena_init(&arenas.tmp, ARENA_DEFAULT_CHUNK_SIZE);
		ctx->arenas = &arenas;
		status = parse(&cursor, ast, ctx, &arenas);
		report_error(ctx);
		if (status == ST_FAILURE)
		{
			ctx->arenas = NULL;
			ft_arena_destroy(&arenas.tmp);
			ft_arena_destroy(&arenas.ast);
			continue ;
		}
		else if (status == ST_EXIT || status == ST_FATAL)
		{
			close_heredocs(ast->left);
			ast->left = NULL;
			ctx->arenas = NULL;
			ft_arena_destroy(&arenas.tmp);
			ft_arena_destroy(&arenas.ast);
			handle_command_termination(status, user_input, ast, ctx);
		}
		status = execute(ast, ctx);
		report_error(ctx);
		if (ast->left)
		{
			close_heredocs(ast->left);
			ast->left = NULL;
		}
		ctx->arenas = NULL;
		ft_arena_destroy(&arenas.tmp);
		ft_arena_destroy(&arenas.ast);
		if (status == ST_FAILURE)
			continue ;
		else if (status == ST_EXIT || status == ST_FATAL)
			handle_command_termination(status, user_input, ast, ctx);
	}
	return (ST_OK);
}

int main(int argc, char **argv, char **envp)
{
	t_ctx		ctx;
	char		*user_input;
	t_node		ast;

	(void)argc;
	(void)argv;
	if (set_signal() == FAILURE)
		return (1);
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	while (1)
	{
		g_signum = 0;
		user_input = shell_read_line("minishell$ ");
		if (user_input == NULL)
			break; // ctrl-D sends EOF, and readline returns NULL receiving EOF.
		if (*user_input)
			add_history(user_input);
		if (g_signum == SIGINT)
			ctx.err.exit_code = 130;
		parse_and_execute(user_input, &ast, &ctx);
		free(user_input);
	}
	dispose_shell(NULL, &ctx);
	return (ctx.err.exit_code);
}
